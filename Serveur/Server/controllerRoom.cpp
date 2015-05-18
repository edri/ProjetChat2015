#include "controllerRoom.h"
#include <QByteArray>

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

void ControllerRoom::createRoom(ModelRoom& room, ChatorClient* client)
{
    // The client must be logged in to create a room
    if (!client->logged)
    {
        client->socket.sendBinaryMessage(_interpretor->sendError(ModelError(ErrorType::ROOM_CREATION, "User not logged in")));
        return;
    }
    
    // Create the room in the database
    room.setIdRoom(_db.createRoom(room));
    
    QSet<quint32> users = room.getUsers();
    
    // Create the room in the onlineRooms structure
    ChatorRoom* newRoom = new ChatorRoom;
    newRoom->id = room.getIdRoom();
    _onlineRooms.insert(newRoom->id, newRoom);
    
    QMap<quint32, ChatorClient*>& connectedUsers = _user->getConnectedUsers();
    
    QMap<quint32, ModelUser> usersData;
    ChatorClient* currentClient;
    
    for (quint32 idUser : users)
    {
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
        client->socket.sendBinaryMessage(data);
    }
}

QMap<quint32, ChatorRoom*>& ControllerRoom::getOnlineRooms()
{
    return _onlineRooms;
}

void ControllerRoom::deleteMessage(const quint32 messageId, ChatorClient* sender)
{
    ModelMessage message = _db.infoMessage(messageId);
    if (sender->logged && message.getIdUser() == sender->id)
    {
        _db.deleteMessage(messageId);
        ChatorRoom* room = _onlineRooms[message.getIdRoom()];
        if (room)
        {
            QByteArray data = _interpretor->deleteMessage(messageId);
            
            for (ChatorClient* client : room->clients)
            {
                client->socket.sendBinaryMessage(data);
            }
        }
    }
}

void ControllerRoom::leaveRoom(const quint32& idRoom, ChatorClient* client)
{
    
}

void ControllerRoom::joinRoom(const quint32& idRoom, ChatorClient* client)
{
    
}

void ControllerRoom::modifyRoom(ModelRoom& room, ChatorClient* client)
{
    
}

void ControllerRoom::deleteRoom(const quint32 roomId, ChatorClient* client)
{
    ChatorRoom* room = _onlineRooms[roomId];
    if (room && _db.infoRoom(roomId).getAdmins().contains(client->id))
    {
        _db.deleteRoom(roomId);
        QByteArray data = _interpretor->deleteRoom(roomId);
        
        for (ChatorClient* currentClient : room->clients)
        {
            currentClient->socket.sendBinaryMessage(data);
            currentClient->rooms.remove(room);
        }
    }
}
