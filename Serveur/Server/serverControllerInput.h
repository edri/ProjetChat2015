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
    
    void receiveMessage(ModelMessage& message, QObject* sender);
    void login(const QString& pseudo, const QString& hashedPWD, QObject* sender);
    void infoUser(ModelUser& user, QObject* sender);
    void room(const ModelRoom& room, bool edited, QObject* sender);
    
    private:
    ControllerUser& _controllerUser;
    ControllerRoom& _controllerRoom;
};

#endif
