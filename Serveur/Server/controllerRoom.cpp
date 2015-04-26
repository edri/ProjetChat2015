#include "controllerRoom.h"

ControllerRoom::ControllerRoom(ControllerDB& db) : _db(db) {}

void ControllerRoom::storeMessage(ModelMessage& message, ChatorClient* client)
{
    ChatorRoom* room = nullptr;
    if (client->logged && message.getIdUser() == client->id && (room = onlineRooms[message.getIdRoom()]) && room->clients.contains(client))
    {
        message.setIdMessage(_db.storeMessage(message));
        message.setDate(QDateTime::currentDateTime());
        
        for (ChatorClient* client : room->clients)
        {
            client->socket.sendBinaryMessage(interpretor->sendMessage(message));
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
    
    for (ChatorRoom* room : currentClient->rooms)
    {
        for (ChatorClient* client : room->clients)
        {
            if (!upToDateClients.contains(client->id))
            {
                qDebug() << "Notification de R/U " << room->id << "/" << client->id;
                upToDateClients.insert(client->id);
                client->socket.sendBinaryMessage(interpretor->connected(user));
            }
        }
    }
}
