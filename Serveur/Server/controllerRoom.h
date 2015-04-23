#ifndef CONTROLLER_ROOM_H
#define CONTROLLER_ROOM_H

#include <QtGlobal>
#include "controllerDB.h"
#include "../../ModeleChator/modelChator.h"

class ControllerRoom
{
    public:
    ControllerRoom(ControllerDB& db);
    void storeMessage(const ModelMessage& message, ChatorClient* client);
    
    private:
    ControllerDB& _db;
    QMap<quint32, ChatorRoom> connectedUsers;
};

#endif
