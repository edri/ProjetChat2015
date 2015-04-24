#include "controllerRoom.h"

ControllerRoom::ControllerRoom(ControllerDB& db) : _db(db) {}

void ControllerRoom::storeMessage(ModelMessage& message, ChatorClient* client)
{
    if (client->logged && message.getIdUser() == client->id)
    {
        message.setIdMessage(_db.storeMessage(message));
        message.setDate(QDateTime::currentDateTime());
        ChatorRoom* room = onlineRooms[message.getIdRoom()];
        
        for (ChatorClient* client : room->clients)
        {
            client->socket.sendBinaryMessage(interpretor->sendMessage(message));
        }
    }
}

void ControllerRoom::userConnected(const ModelUser& user, ChatorClient& currentClient)
{
    // Aussi envoyer le modeluser? je pense oui
    Q_UNUSED(user);
    
    QSet<quint32> rooms = user.getRooms();
    
    for (quint32 idRoom : rooms)
    {
        ChatorRoom* currentRoom;
        if (!onlineRooms.contains(idRoom))
        {
            currentRoom = new ChatorRoom({currentClient.id, 0, &currentClient});
            onlineRooms.insert(idRoom, currentRoom);
        }
        else
        {
            currentRoom = onlineRooms[idRoom];
        }
        
        currentClient->rooms.append(currentRoom);
    }
    
    for (ChatorRoom* room : currentClient->rooms)
    {
        for (ChatorClient* client : room->clients)
        {
            client->socket.sendBinaryMessage(interpretor->connected(user));
            //client->socket.sendBinaryMessage(interpretor->join(currentClient.id, room->id));
        }
    }
}
