#include "controllerRoom.h"

ControllerRoom::ControllerRoom(ControllerDB& db) : _db(db) {}

void ControllerRoom::storeMessage(ModelMessage& message, ChatorClient* client)
{
    if (client->logged && message.getIdUser() == client->id)
    {
        message.setIdMessage(_db.storeMessage(message));
        message.setDate(QDateTime::currentDateTime());
        ChatorRoom& room = onlineRooms[message.getIdRoom()];
        
        for (ChatorClient* client : room.clients)
        {
            client->socket.sendBinaryMessage(interpretor->sendMessage(message));
        }
    }
}

void ControllerRoom::userConnected(const ModelUser& user, const ChatorClient& currentClient)
{
    // Aussi envoyer le modeluser? je pense oui
    Q_UNUSED(user);
    
    for (ChatorRoom* room : currentClient.rooms)
    {
        for (ChatorClient* client : room->clients)
        {
            client->socket.sendBinaryMessage(interpretor->join(currentClient.id, room->id));
        }
    }
}
