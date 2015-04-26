#include <QtCore/QDebug>
#include "clientControllerInput.h"

ClientControllerInput::ClientControllerInput()
{
    _controllerChat = nullptr;
    _controllerUser = nullptr;
    _controllerRoom = nullptr;
}

ClientControllerInput::ClientControllerInput(ControllerChat* controllerChat, ControllerUser* controllerUser, ControllerRoom* controllerRoom) : _controllerChat(controllerChat), _controllerUser(controllerUser), _controllerRoom(controllerRoom) {}


void ClientControllerInput::controllerChat(ControllerChat* controllerChat)
{
    _controllerChat = controllerChat;
}
void ClientControllerInput::controllerUser(ControllerUser* controllerUser)
{
    _controllerUser = controllerUser;
}

void ClientControllerInput::controllerRoom(ControllerRoom* controllerRoom)
{
    _controllerRoom = controllerRoom;
}

void ClientControllerInput::receiveMessage(ModelMessage& message, QObject* sender)
{
    // Sender is unused in the client controller.
    Q_UNUSED(sender);
     //ControllerChat Missing method
    //_controllerChat.receiveMessage(message);
}

void ClientControllerInput::infoUser(ModelUser& user, QObject* sender)
{
    // Sender is unused in the client controller.
    Q_UNUSED(sender);
    
    qDebug() << "Info recue: " << user.getIdUser() << ", " << user.getUserName();
    
    //ControllerUser Missing method
    //_controllerUser.infoUser(user);
}
