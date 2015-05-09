#include "controllerRoom.h"
#include <QByteArray>

ControllerRoom::ControllerRoom(ControllerDB& db) : _db(db) {}

void ControllerRoom::storeMessage(ModelMessage& message, const bool edited, ChatorClient* client)
{
    ChatorRoom* room = nullptr;
    qDebug() << "Message à stocker: " << message.getContent() << " dans chambre " << message.getIdRoom() << " de user " << message.getIdUser();
    if (client->logged && message.getIdUser() == client->id && (room = onlineRooms[message.getIdRoom()]) && room->clients.contains(client))
    {
        if (edited)
        {
            _db.editMessage(message);
            message.setEditionDate(QDateTime::currentDateTime());
        }
        else
        {
            message.setIdMessage(_db.storeMessage(message));
            message.setDate(QDateTime::currentDateTime());
        }
        
        QByteArray data = _interpretor->sendMessage(message, edited);
        
        for (ChatorClient* client : room->clients)
        {
            qDebug() << "envoi du message au client " << client->id;
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
        
        if (!onlineRooms.contains(idRoom))
        {
            qDebug() << "Mise en ligne de la salle " << idRoom;
            // If it is not, we add it
            currentRoom = new ChatorRoom;
            currentRoom->id = idRoom;
            onlineRooms.insert(idRoom, currentRoom);
        }
        else
        {
            qDebug() << "Salle déjà en ligne " << idRoom;
            // If this room is already online, we insert the user in it
            currentRoom = onlineRooms[idRoom];
        }
        
        // Then, we store the pointer to this client and bind it to the room
        currentClient->rooms.insert(currentRoom);
        currentRoom->clients.insert(currentClient);
        }
    
    QSet<quint32> upToDateClients;
    QByteArray data = _interpretor->connected(user);
    
    for (ChatorRoom* room : currentClient->rooms)
    {
        for (ChatorClient* client : room->clients)
        {
            if (!upToDateClients.contains(client->id))
            {
                qDebug() << "Notification de R/U " << room->id << "/" << client->id;
                upToDateClients.insert(client->id);
                client->socket.sendBinaryMessage(data);
            }
        }
    }
}

void ControllerRoom::createRoom(ModelRoom& room, ChatorClient* client)
{
    if (!client->logged)
    {
        client->socket.sendBinaryMessage(_interpretor->sendError(ModelError(ErrorType::ROOM_CREATION, "User not logged in")));
        return;
    }
    
    room.setIdRoom(_db.createRoom(room));
    
    QSet<quint32> users = room.getUsers();
    
    ChatorRoom* newRoom = new ChatorRoom;
    newRoom->id = room.getIdRoom();
    onlineRooms.insert(newRoom->id, newRoom);
    
    QMap<quint32, ChatorClient*>& connectedUsers = _user->getConnectedUsers();
    
    QMap<quint32, ModelUser> usersData;
    ChatorClient* currentClient;
    
    for (quint32 idUser : users)
    {
        usersData.insert(idUser, _db.info(idUser));
        if (connectedUsers.contains(idUser))
        {
            currentClient = connectedUsers[idUser];
            currentClient->rooms.insert(newRoom);
            newRoom->clients.insert(currentClient);
        }
    }
    
    QMap<quint32, ModelRoom> roomToSend;
    roomToSend.insert(newRoom->id, room);
    QByteArray data = _interpretor->join(roomToSend, usersData);
    
    for (ChatorClient* client : newRoom->clients)
    {
        client->socket.sendBinaryMessage(data);
    }
}
