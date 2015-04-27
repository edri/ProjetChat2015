#ifndef CONTROLLER_ROOM_H
#define CONTROLLER_ROOM_H

#include <QtGlobal>
#include "controllerDB.h"
#include "../../ModeleChator/modelChator.h"
#include "../Interpretor/interpretor.h"

class ControllerRoom
{
    public:
    ControllerRoom(ControllerDB& db);
    void storeMessage(ModelMessage& message, ChatorClient* client);
    void userConnected(const ModelUser& user, ChatorClient* client);
    void createRoom(ModelRoom& room, ChatorClient* client);
    Interpretor* interpretor;
    
    private:
    ControllerDB& _db;
    QMap<quint32, ChatorRoom*> onlineRooms;
};

#endif
