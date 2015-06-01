/*
     * implements controllerRoom.h
*/

#include "controllerRoom.h"
#include <QByteArray>
#include <QList>
#include <QPair>

ControllerRoom::ControllerRoom(ControllerDB& db) : _db(db) {}

void ControllerRoom::processMessage(ModelMessage& message, const bool edited, ChatorClient* client)
{
    ChatorRoom* room = nullptr;
    
    // We check that it is a genuine message: the client must be logged in, the client must be the sender of the message, the room must be online, the client has to be registered in that room
    if (client->logged && message.getIdUser() == client->id && (room = _onlineRooms[message.getIdRoom()]) && room->clients.contains(client))
    {
        // The message can either be a new one or an old one that is being edited
        if (edited)
        {
            _db.editMessage(message);
        }
        else
        {
            message.setIdMessage(_db.storeMessage(message));
            message.setDate(QDateTime::currentDateTime());
        }
        
        // The modified date must be set to now anyway
        message.setEditionDate(QDateTime::currentDateTime());
        
        // Build the packet to send
        QByteArray data = _interpretor->sendMessage(message, edited);
        
        // Send the packet to every online client of the room
        for (ChatorClient* client : room->clients)
        {
            client->socket.sendBinaryMessage(data);
        }
    }
}

void ControllerRoom::userConnected(const ModelUser& user, ChatorClient* currentClient)
{    
    // Get the ids of the rooms where this user is connected
    QSet<quint32> rooms = user.getRooms();
    
    for (quint32 idRoom : rooms)
    {
        // Check if the room is already online
        ChatorRoom* currentRoom;
        
        if (!_onlineRooms.contains(idRoom))
        {
            qDebug() << "The room goes online " << idRoom;
            
            // If it is not, we add it
            currentRoom = new ChatorRoom;
            currentRoom->id = idRoom;
            _onlineRooms.insert(idRoom, currentRoom);
        }
        else
        {
            // If this room is already online, we insert the user in it
            currentRoom = _onlineRooms[idRoom];
        }
        
        // Then, we store the pointer to this client and bind it to the room
        currentClient->rooms.insert(currentRoom);
        currentRoom->clients.insert(currentClient);
    }
    
    // We will store the clients that already have been informed
    QSet<quint32> upToDateClients;
    upToDateClients.insert(currentClient->id);
    
    // Build the packet
    QByteArray data = _interpretor->connected(user);
    
    // We must inform every client within every room related to the user
    for (ChatorRoom* room : currentClient->rooms)
    {
        for (ChatorClient* client : room->clients)
        {
            if (!upToDateClients.contains(client->id))
            {
                // If this client hasn't been informed, we send him the packet
                upToDateClients.insert(client->id);
                client->socket.sendBinaryMessage(data);
            }
        }
    }
}

void ControllerRoom::createRoom(ModelRoom& room, const QMap<quint32, QByteArray>& usersAndKeys, ChatorClient* client)
{
    // The client must be logged in to create a room
    if (!client->logged)
    {
        client->socket.sendBinaryMessage(_interpretor->sendError(ModelError(ErrorType::ROOM_CREATION, "User not logged in")));
        return;
    }
    
    // Create the room in the database
    room.setIdRoom(_db.createRoom(room));
    
    // Create the room in the onlineRooms structure
    ChatorRoom* newRoom = new ChatorRoom;
    newRoom->id = room.getIdRoom();
    _onlineRooms.insert(newRoom->id, newRoom);
    
    QMap<quint32, ChatorClient*>& connectedUsers = _user->_connectedUsers;
    
    QMap<quint32, ModelUser> usersData;
    ChatorClient* currentClient;
    
    bool isPrivate = room.isPrivate();
    
    qDebug() << "Creation of a new room: " << room.getName();
    
    for (quint32 idUser : room.getUsers())
    {
        // If the room is private, we have to store the AES key for each user        
        if (isPrivate)
        {
            _db.setKey(idUser, newRoom->id, usersAndKeys[idUser]);
        }
        
        // For every user in the new room, we have to fetch its ModelUser
        usersData.insert(idUser, _db.info(idUser));
        
        // If this user is online
        if (connectedUsers.contains(idUser))
        {
            // We add it in the room and link it to it
            currentClient = connectedUsers[idUser];
            currentClient->rooms.insert(newRoom);
            newRoom->clients.insert(currentClient);
        }
    }
    
    // Prepare the informations to be sent
    QMap<quint32, ModelRoom> roomToSend;
    roomToSend.insert(newRoom->id, room);
    
    // Build the packet
    QByteArray data = _interpretor->join(roomToSend, usersData);
    
    // Send the packet to every client
    for (ChatorClient* client : newRoom->clients)
    {
        if (isPrivate)
        {
            QDataStream stream(usersAndKeys[client->id]);
            AESKey aesKey;
            stream >> aesKey;
            
            roomToSend.first().setKey(aesKey);
            data = _interpretor->join(roomToSend, usersData);
        }

        client->socket.sendBinaryMessage(data);
    }
}


void ControllerRoom::deleteMessage(const quint32 roomId, const quint32 messageId, ChatorClient* client)
{
    // We fetch the ModelMessage
    ModelMessage message = _db.infoMessage(messageId);
    
    // We check that the user is logged and that he is the genuine author of the message
    if (client->logged && message.getIdUser() == client->id)
    {
        // We actually delete the message
        _db.deleteMessage(messageId);
        ChatorRoom* room = _onlineRooms[roomId];
        
        // Now we tell every client in the room that this message was deleted
        if (room)
        {
            QByteArray data = _interpretor->deleteMessage(roomId, messageId);
            
            for (ChatorClient* client : room->clients)
            {
                client->socket.sendBinaryMessage(data);
            }
        }
    }
}

void ControllerRoom::leaveRoom(const quint32 idUser, const quint32 idRoom, ChatorClient* client)
{
    // We check that the client is genuine
    if (idUser != client->id)
    {
        client->socket.sendBinaryMessage(_interpretor->sendError(ModelError(ErrorType::AUTH_ERROR, "Who are you?")));
        return;
    }
    
    // We actually remove the membership in the database
    _db.leaveRoom(idUser, idRoom);
    
    // Now we have to look for the ChatorRoom in the rooms of this user
    for (ChatorRoom* room : client->rooms)
    {
        // We found it!
        if (room->id == idRoom)
        {
            // We remove the room from the list    
            client->rooms.remove(room);
            
            // We have to tell everyone in the room that this user leaves it (including the leaving one)
            QByteArray data = _interpretor->leave(client->id, idRoom);
            
            for (ChatorClient* member : room->clients)
            {
                member->socket.sendBinaryMessage(data);
            }
            
            // Now we can remove the user from the list of the room
            room->clients.remove(client);
            
            // If the room is now empty, we have to delete it
            if (room->clients.empty())
            {
                _onlineRooms.remove(idRoom);
                delete room;
            }
            return;
        }
    }
}

void ControllerRoom::joinRoom(const quint32 idRoom, ChatorClient* client)
{
    qDebug() << "The user " << client->id << " requests access to room " << idRoom;
    
    ModelRoom room = _db.infoRoom(idRoom);
    
    // If the room is public, we can add the user automatically
    if (!room.isPrivate())
    {
        QSet<quint32> newUser;
        newUser << client->id;
        
        // We create a membership and add the user to the room
        _db.modifyMembership(idRoom, newUser);
        room.addUser(client->id);
        
        ChatorRoom* currentRoom;
        
        // If the room isn't online
        if (!_onlineRooms.contains(idRoom))
        {
            qDebug() << "The room goes online " << idRoom;
            
            // If it is not, we add it
            currentRoom = new ChatorRoom;
            currentRoom->id = idRoom;
            _onlineRooms.insert(idRoom, currentRoom);
        }
        else
        {
            // If this room is already online, we insert the user in it
            currentRoom = _onlineRooms[idRoom];
        }
        
        // Then, we store the pointer to this client and bind it to the room
        client->rooms.insert(currentRoom);
        currentRoom->clients.insert(client);
        
        
        // Now we have to warn every user of the room that a new one has shown up, and we have to send the modeluser of everyone
        QMap<quint32, ModelRoom> rooms;
        rooms.insert(idRoom, room);
        
        QMap<quint32, ModelUser> users;
        
        for (quint32 member : room.getUsers())
        {
            users.insert(member, _db.info(member));
        }
        
        QByteArray data = _interpretor->join(rooms, users);
        
        for (ChatorClient* c : currentRoom->clients)
        {
            c->socket.sendBinaryMessage(data);
        }
    }
    
    // If the room is private, we create a request in the database
    else if (_db.requestAccess(client->id, idRoom))
    {
        ModelRoom room = _db.infoRoom(idRoom);
        ChatorRoom* currentRoom = _onlineRooms[idRoom];
        
        // If the room is online, we can send the request to the admins if there are some
        if (currentRoom)
        {
            const QSet<quint32>& admins = room.getAdmins();
            
            // We have to get the public key of the user to send the request
            QByteArray keySalt, publicKey, privateKey;
            _db.getCryptoData(client->id, keySalt, publicKey, privateKey);
            QByteArray data = _interpretor->request(idRoom, _db.info(client->id), publicKey, false);
            
            // We send a request to every online admin in the room
            for (ChatorClient* c : currentRoom->clients)
            {
                if (admins.contains(c->id))
                {
                    c->socket.sendBinaryMessage(data);
                }
            }
        }
    }
}

void ControllerRoom::modifyRoom(ModelRoom& room, const QMap<quint32, QByteArray>& usersAndKeys, ChatorClient* client)
{
    ChatorRoom* onlineRoom = _onlineRooms[room.getIdRoom()];
    
    // We check that the room is online and that the user is admin for this room
    if (!onlineRoom || !_db.infoRoom(onlineRoom->id).getAdmins().contains(client->id))
    {
        client->socket.sendBinaryMessage(_interpretor->sendError(ModelError(ErrorType::AUTH_ERROR, "incorrect user")));
        return;
    }
    
    // To compute the difference between the new and the old version of the room, we get the old version
    ModelRoom oldRoom = _db.infoRoom(room.getIdRoom());
    
    // We compute the difference sets
    QSet<quint32> newUsers = room.getUsers() - oldRoom.getUsers();
    QSet<quint32> removedUsers = oldRoom.getUsers() - room.getUsers();
    QSet<quint32> newAdmins = room.getAdmins() - oldRoom.getAdmins();
    QSet<quint32> removedAdmins = oldRoom.getAdmins() - room.getAdmins();
    
    // We modify the room in the database and the memberships
    _db.modifyRoom(room);
    _db.modifyMembership(room.getIdRoom(), newUsers, removedUsers, newAdmins, removedAdmins, usersAndKeys);
    
    // We process every leaving client. Unfortunately, we have to do it in two stages because we cannot remove something from a set while browsing it.
    QList<ChatorClient*> clientsToRemove;
    
    for (ChatorClient* currentClient : onlineRoom->clients)
    {
        if (removedUsers.contains(currentClient->id))
        {
            clientsToRemove.append(currentClient);
        }
    }
    
    // Now, we can simply call leave for every removed client
    for (ChatorClient* clientToRemove : clientsToRemove)
    {
        leaveRoom(clientToRemove->id, onlineRoom->id, clientToRemove);
    }
    
    // We send the new version of the room to every client remaining in it
    QByteArray data = _interpretor->room(room, QMap<quint32, QByteArray>(), true);
    
    for (ChatorClient* currentClient : onlineRoom->clients)
    {
        currentClient->socket.sendBinaryMessage(data);
    }
    
    // We have to build a join packet so that every client knows every other
    QMap<quint32, ModelRoom> rooms;
    rooms.insert(room.getIdRoom(), room);
    QMap<quint32, ModelUser> users;
    for (quint32 idUser : room.getUsers())
    {
        if (!users.contains(idUser))
        {
            // Get the informations of the user
            users.insert(idUser, _db.info(idUser));
        }
    }
    
    // We have to bind the pointers of the new users to the rooms
    for (ChatorClient* onlineClient : _user->_connectedUsers)
    {
        if (newUsers.contains(onlineClient->id))
        {
            onlineRoom->clients.insert(onlineClient);
            onlineClient->rooms.insert(onlineRoom);
        }
    }
    
    // We can finally build and send the join packet
    data = _interpretor->join(rooms, users);
    
    for (ChatorClient* onlineClient : onlineRoom->clients)
    {
        // If we are about to send it to a new user, we add its own version of the AES key
        if (newUsers.contains(onlineClient->id))
        {
            QDataStream stream(usersAndKeys[onlineClient->id]);
            AESKey aesKey;
            stream >> aesKey;
            rooms.first().setKey(aesKey);
            
            onlineClient->socket.sendBinaryMessage(_interpretor->join(rooms, users));
        }
        else
        {
            onlineClient->socket.sendBinaryMessage(data);
        }
    }
}

void ControllerRoom::deleteRoom(const quint32 roomId, ChatorClient* client)
{
    ChatorRoom* room = _onlineRooms[roomId];
    
    // We check that the room is online and that the user is a genuine admin
    if (room && _db.infoRoom(roomId).getAdmins().contains(client->id))
    {
        qDebug() << "Destroying the room " << roomId;
        _db.deleteRoom(roomId);
        
        // Before actually destroying it from the memory, we have to warn all of its clients
        QByteArray data = _interpretor->deleteRoom(roomId);
        for (ChatorClient* currentClient : room->clients)
        {
            currentClient->socket.sendBinaryMessage(data);
            currentClient->rooms.remove(room);
        }
        
        // The room is destroyed
        _onlineRooms.remove(roomId);
        delete room;
    }
}

void ControllerRoom::acceptOrDeny(const quint32 idRoom, const quint32 idUser, const QByteArray& key, const bool accepted)
{
    // We update the database with the answer
    _db.acceptOrDeny(idRoom, idUser, key, accepted);
    
    if (accepted)
    {
        // The membership of the room has changed, so we have to send a join packet so that everyone knows everyone
        ModelRoom room = _db.infoRoom(idRoom);
        QMap<quint32, ModelRoom> rooms;
        rooms.insert(room.getIdRoom(), room);
        QMap<quint32, ModelUser> users;
        
        for (quint32 idUser : room.getUsers())
        {
            if (!users.contains(idUser))
            {
                // Get the informations of the user
                users.insert(idUser, _db.info(idUser));
            }
        }
        
        ChatorRoom* onlineRoom = _onlineRooms[idRoom];
        
        // We bind the pointer of the user to the room
        for (ChatorClient* onlineClient : _user->_connectedUsers)
        {
            if (onlineClient->id == idUser)
            {
                onlineRoom->clients.insert(onlineClient);
                onlineClient->rooms.insert(onlineRoom);
                break;
            }
        }
        
        // We actually build and send the join
        QByteArray data = _interpretor->join(rooms, users);
        
        for (ChatorClient* onlineClient : onlineRoom->clients)
        {
            // If we are about to send it to the new user, we add the AES key
            if (onlineClient->id == idUser)
            {
                QDataStream stream(key);
                AESKey aesKey;
                stream >> aesKey;
                rooms.first().setKey(aesKey);
                
                onlineClient->socket.sendBinaryMessage(_interpretor->join(rooms, users));
            }
            else
            {
                onlineClient->socket.sendBinaryMessage(data);
            }
        }
    }
}
