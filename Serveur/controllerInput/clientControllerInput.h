#ifndef CLIENT_CONTROLLER_INPUT_H
#define CLIENT_CONTROLLER_INPUT_H

#include "controllerInput.h"
#include "../../Chat/controllerchat.h"
#include "../../Salle/Room/controllerRoom.h"
#include "../../User/controllerUser.h"

class ControllerUser;

class ClientControllerInput : public ControllerInput
{
    ControllerChat& _controllerChat;
    ControllerUser& _controllerUser;
    ControllerRoom& _controllerRoom;
    // Error controller ?
    // Main controller ?
    
    public:
    
    ClientControllerInput(ControllerChat& controllerChat, ControllerUser& controllerUser, ControllerRoom& controllerRoom);
    
    // Method called when a message is received by the client.
    void receiveMessage(ModelMessage& message, QObject* sender);
    // Method called when user's info are received by the client when loging in.
    void infoUser(ModelUser& user, QObject* sender);
    
    
};

#endif // CLIENT_CONTROLLER_INPUT_H
