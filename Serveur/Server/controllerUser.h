#ifndef CONTROLLER_USER_H
#define CONTROLLER_USER_H

#include "controllerDB.h"
#include "serverModel.h"
#include "../Interpretor/interpretor.h"
#include "controllerRoom.h"

class ControllerUser
{
    public:
    ControllerUser(ControllerDB& db, ControllerRoom& room);
    void login(const QString& pseudo, const QString& hashedPWD, ChatorClient* client);
    Interpretor* interpretor;
    
    private:
    ControllerDB& _db;
    ControllerRoom& _room;
    QMap<quint32, ChatorClient*> connectedUsers;
};

#endif
