#include "controllerRoom.h"

ControllerRoom::ControllerRoom(ControllerDB& db) : _db(db) {}

void ControllerRoom::storeMessage(const ModelMessage& message, ChatorClient* client)
{
    if (message.getIdUser() == client->id)
    {
        quint32 idMessage = _db.storeMessage(message);
    }
}
