#include "controllerRoom.h"
#include <QByteArray>
#include <QList>
#include <QPair>

ControllerRoom::ControllerRoom(ControllerDB& db) : _db(db) {}

void ControllerRoom::processMessage(ModelMessage& message, const bool edited, ChatorClient* client)
{
    ChatorRoom* room = nullptr;
    
    // Test
    qDebug() << "Message à stocker: " << message.getIdMessage() << ", " << message.getContent() << " dans chambre " << message.getIdRoom() << " de user " << message.getIdUser() << ", cree" << message.getDate();
    // until here
    
    // We check that it is a genuine message: the client must be logged in, the client must be the sender of the message, the room must be online, the client has to be registered in that room
    if (client->logged && message.getIdUser() == client->id && (room = _onlineRooms[message.getIdRoom()]) && room->clients.contains(client))
    {
        // The message can either be a new one or an old one that is being edited
        if (edited)
        {
            _db.editMessage(message);
            qDebug() << "Modification du message " << message.getIdMessage() << ", cree " << message.getDate() << ", modifie " << message.getEditionDate();
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
            // Test
            qDebug() << "envoi du message au client " << client->id;
            // until here
            
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
            qDebug() << "Mise en ligne de la salle " << idRoom;
            // If it is not, we add it
            currentRoom = new ChatorRoom;
            currentRoom->id = idRoom;
            _onlineRooms.insert(idRoom, currentRoom);
        }
        else
        {
            qDebug() << "Salle déjà en ligne " << idRoom;
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
                // TEst
                qDebug() << "Notification de R/U " << room->id << "/" << client->id;
                // until here
                
                // If this client hasn't benn informed, we send him the packet
                upToDateClients.insert(client->id);
                client->socket.sendBinaryMessage(data);
            }
        }
    }
}

void ControllerRoom::createRoom(ModelRoom& room, QList<quint32> usersIds, QList<QPair<QByteArray, QByteArray>> cryptedKeys, ChatorClient* client)
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
    
    int size = usersIds.size();
    bool isPrivate = room.isPrivate();
    quint32 idUser;
    
    qDebug() << "Création de la salle " << room.getName() << " avec users: " << usersIds.size();
    
    //for (quint32 idUser : usersIds)
    for (int i = 0; i < size; i++)
    {
        idUser = usersIds[i];
         qDebug() << "Traitement de l'user " << idUser;
        
        if (isPrivate)
        {
            QByteArray aesKey;
            QDataStream stream(&aesKey, QIODevice::WriteOnly);
            stream << cryptedKeys[i].first << cryptedKeys[i].second;
            qDebug() << "A la creation, le truc fait " << aesKey.size();
            qDebug() << "La clé originale fait " << cryptedKeys[i].first.size() << "|" << cryptedKeys[i].first.size();
            _db.setKey(idUser, newRoom->id, aesKey);
        }
        
        // For every user in the new room, we have to fetch its ModelUser
        usersData.insert(idUser, _db.info(idUser));
        
        // If this user is online
        if (connectedUsers.contains(idUser))
        {
            qDebug() << "Oui, il est online";
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
    
    int clientIndex;
    // Send the packet to every client
    for (ChatorClient* client : newRoom->clients)
    {
        if (isPrivate)
        {
            QByteArray aesKeyAndIV;
            QDataStream writeStream(&aesKeyAndIV, QIODevice::WriteOnly);
            clientIndex = usersIds.indexOf(client->id);
            qDebug() << "Index de l'utilisateur actuel" << clientIndex;
            qDebug() << "Clé de l'utilisateur actuel" << cryptedKeys[clientIndex].first.size();
            qDebug() << "IV de l'utilisateur actuel" << cryptedKeys[clientIndex].second.size();
            writeStream << cryptedKeys[clientIndex].first << cryptedKeys[clientIndex].second;
            
            qDebug() << "La chose contient " << aesKeyAndIV.size();
            
            QDataStream readStream(aesKeyAndIV);
            AESKey aesKey;
            readStream >> aesKey;
            qDebug() << "L'aeskey contient: " << aesKey.key.size() << "|" << aesKey.initializationVector.size();
            roomToSend.first().setKey(aesKey);
            data = _interpretor->join(roomToSend, usersData);
        }
        qDebug() << "Envoi de la notif qui fait " << data.size();
        client->socket.sendBinaryMessage(data);
    }
}


void ControllerRoom::deleteMessage(const quint32 roomId, const quint32 messageId, ChatorClient* client)
{
    ModelMessage message = _db.infoMessage(messageId);
    if (client->logged && message.getIdUser() == client->id)
    {
        _db.deleteMessage(messageId);
        ChatorRoom* room = _onlineRooms[roomId];
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
    if (idUser != client->id)
    {
        client->socket.sendBinaryMessage(_interpretor->sendError(ModelError(ErrorType::ROOM_CREATION, "Who are you?")));
        return;
    }
    
    _db.leaveRoom(idUser, idRoom);
    
    for (ChatorRoom* room : client->rooms)
    {
        if (room->id == idRoom)
        {
            QByteArray data = _interpretor->leave(client->id, idRoom);
            
            for (ChatorClient* member : room->clients)
            {
                member->socket.sendBinaryMessage(data);
            }
            
            room->clients.remove(client);
            
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
    qDebug() << "User " << client->id << " souhaite rejoindre salle " << idRoom;
    
    ModelRoom room = _db.infoRoom(idRoom);
    
    if (!room.isPrivate())
    {
        QSet<quint32> newUser = newUser << client->id;
        _db.modifyMembership(idRoom, newUser);
        room.addUser(client->id);
        ChatorRoom* currentRoom;
        
        if (!_onlineRooms.contains(idRoom))
        {
            qDebug() << "Mise en ligne de la salle " << idRoom;
            // If it is not, we add it
            currentRoom = new ChatorRoom;
            currentRoom->id = idRoom;
            _onlineRooms.insert(idRoom, currentRoom);
        }
        else
        {
            qDebug() << "Salle déjà en ligne " << idRoom;
            // If this room is already online, we insert the user in it
            currentRoom = _onlineRooms[idRoom];
        }
        
        // Then, we store the pointer to this client and bind it to the room
        client->rooms.insert(currentRoom);
        currentRoom->clients.insert(client);
        
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
    else
    {
        ModelRoom room = _db.infoRoom(idRoom);
        _db.requestAccess(client->id, idRoom);
        
        // Les ennuis commencent
        ChatorRoom* currentRoom = _onlineRooms[idRoom];
        
        if (currentRoom)
        {
            const QSet<quint32>& admins = room.getAdmins();
            QByteArray keySalt, publicKey, privateKey;
            _db.getCryptoData(client->id, keySalt, publicKey, privateKey);
            QByteArray data = _interpretor->request(idRoom, _db.info(client->id), publicKey, false);
            
            for (ChatorClient* c : currentRoom->clients)
            {
                if (admins.contains(c->id))
                {
                    // ENVOYER UNE NOTIF
                    c->socket.sendBinaryMessage(data);
                }
            }
        }
    }
}

// IL FAUDRAIT PLUTÔT UNE QMAP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// USERSIDS NE SERT À RIEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!¨¨
void ControllerRoom::modifyRoom(ModelRoom& room, QList<quint32> usersIds, QList<QPair<QByteArray, QByteArray>> cryptedKeys, ChatorClient* client)
{
    // Construction moche d'une Qmap
    // BERK BERK BERK
    QMap<quint32, QByteArray> usersAndKeys;
    for (int i = 0; i < usersIds.size(); i++)
    {
        QByteArray aesKey;
        QDataStream stream(&aesKey, QIODevice::WriteOnly);
        stream << cryptedKeys[i].first << cryptedKeys[i].second;
        usersAndKeys.insert(usersIds[i], aesKey);
    }
    // -------
    
    ChatorRoom* onlineRoom = _onlineRooms[room.getIdRoom()];
    
    if (!onlineRoom || !_db.infoRoom(onlineRoom->id).getAdmins().contains(client->id))
    {
        client->socket.sendBinaryMessage(_interpretor->sendError(ModelError(ErrorType::AUTH_ERROR, "incorrect user")));
        return;
    }
    
    ModelRoom oldRoom = _db.infoRoom(room.getIdRoom());
    
    QSet<quint32> newUsers = room.getUsers() - oldRoom.getUsers();
    QSet<quint32> removedUsers = oldRoom.getUsers() - room.getUsers();
    QSet<quint32> newAdmins = room.getAdmins() - oldRoom.getAdmins();
    QSet<quint32> removedAdmins = oldRoom.getAdmins() - room.getAdmins();
    
    _db.modifyRoom(room);
    _db.modifyMembership(room.getIdRoom(), newUsers, removedUsers, newAdmins, removedAdmins, usersAndKeys);
    
    // Le paquet n'existe pas encore
    QList<QPair<QByteArray, QByteArray>> prout2;
    QList<quint32> prout;
    QByteArray data = _interpretor->room(room, prout, prout2, true);
    //QByteArray data;
    
    for (ChatorClient* currentClient : onlineRoom->clients)
    {
        currentClient->socket.sendBinaryMessage(data);
        if (removedUsers.contains(currentClient->id))
        {
            leaveRoom(currentClient->id, onlineRoom->id, currentClient);
        }
    }
    
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
    
    data = _interpretor->join(rooms, users);
    
    for (ChatorClient* onlineClient : _user->_connectedUsers)
    {
        if (newUsers.contains(onlineClient->id))
        {
            onlineRoom->clients.insert(onlineClient);
            onlineClient->rooms.insert(onlineRoom);
            onlineClient->socket.sendBinaryMessage(data);
        }
    }
}

void ControllerRoom::deleteRoom(const quint32 roomId, ChatorClient* client)
{
    ChatorRoom* room = _onlineRooms[roomId];
    if (room && _db.infoRoom(roomId).getAdmins().contains(client->id))
    {
        qDebug() << "suppression de la salle " << roomId;
        _db.deleteRoom(roomId);
        QByteArray data = _interpretor->deleteRoom(roomId);
        
        for (ChatorClient* currentClient : room->clients)
        {
            qDebug() << "Envoi de l'information a " << currentClient->id;
            currentClient->socket.sendBinaryMessage(data);
            currentClient->rooms.remove(room);
        }
        
        _onlineRooms.remove(roomId);
        delete room;
    }
}

void ControllerRoom::acceptOrDeny(const quint32 idRoom, const quint32 idUser, const QByteArray& key, const bool accepted, ChatorClient* client)
{
    _db.acceptOrDeny(idRoom, idUser, key, accepted);
    
    if (accepted)
    {
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
        
        for (ChatorClient* onlineClient : _user->_connectedUsers)
        {
            if (onlineClient->id == idUser)
            {
                onlineRoom->clients.insert(onlineClient);
                onlineClient->rooms.insert(onlineRoom);
                break;
            }
        }
        
        for (ChatorClient* onlineClient : onlineRoom->clients)
        {
            onlineClient->socket.sendBinaryMessage(_interpretor->join(rooms, users));
        }
    }
}
