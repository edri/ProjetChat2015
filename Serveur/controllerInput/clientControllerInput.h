#ifndef CLIENT_CONTROLLER_INPUT_H
#define CLIENT_CONTROLLER_INPUT_H

#include "controllerInput.h"
#include "../../Chat/controllerChat.h"
#include "../../Salle/Room/controllerRoom.h"
#include "../../User/controllerUser.h"

class ControllerUser;
class ControllerChat;

class ClientControllerInput : public ControllerInput
{
    ControllerChat* _controllerChat;
    ControllerUser* _controllerUser;
    ControllerRoom* _controllerRoom;
    // Error controller ?
    // Main controller ?
    
    public:
    
    // Constructors
    ClientControllerInput();
    ClientControllerInput(ControllerChat* controllerChat, ControllerUser* controllerUser, ControllerRoom* controllerRoom);
    
    // Setters
    void controllerChat(ControllerChat* controllerChat);
    void controllerUser(ControllerUser* controlleruser);
    void controllerRoom(ControllerRoom* controllerRoom);
    
    // Method called when a message is received by the client.
    void receiveMessage(ModelMessage& message, QObject* sender);
    // Method called when user's info are received by the client when loging in.
    void infoUser(ModelUser& user, QObject* sender);
    // Method called when confirmation of creation or edition of a room is received.
    void room(ModelRoom& room, bool edited, QObject* sender);
    // Method called when the client has to add a room and its users
    void join(const QMap<quint32, ModelRoom>& rooms, const QMap<quint32, ModelUser>& users, QObject* sender);
    
    
};

#endif // CLIENT_CONTROLLER_INPUT_H
