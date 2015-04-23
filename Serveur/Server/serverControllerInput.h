#ifndef SERVER_CONTROLLER_INPUT_H
#define SERVER_CONTROLLER_INPUT_H

#include "../controllerInput/controllerInput.h"
#include "controllerUser.h"
#include "controllerRoom.h"
#include "serverModel.h"

class ServerControllerInput : public ControllerInput
{
    Q_OBJECT
    
    public:
    ServerControllerInput(ControllerUser& controllerUser, ControllerRoom& controllerRoom);
    
    virtual void receiveMessage(ModelMessage& message, QObject* sender);
    virtual void login(const QString& pseudo, const QString& hashedPWD, QObject* sender);
    
    private:
    ControllerUser& _controllerUser;
    ControllerRoom& _controllerRoom;
};

#endif
