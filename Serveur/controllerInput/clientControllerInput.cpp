#include <QtCore/QDebug>
#include "controllerInput.h"


ClientControllerInput::ClientControllerInput(ControllerChat& controllerChat, ControllerUser& controllerUser, ControllerRoom& controllerRoom) : _controllerChat(controllerChat), _controllerUser(controllerUser), _controllerRoom(controllerRoom) {}


void ClientControllerInput::receiveMessage(ModelMessage& message, QObject* sender)
{
    // Sender is unused in the client controller.
    Q_UNUSED(sender);
     ControllerChat Missing method
    _controllerChat.receiveMessage(message);
}

void ClientControllerInput::infoUser(ModelUser& user, QObject* sender)
{
    // Sender is unused in the client controller.
    Q_UNUSED(sender);
    
    qDebug() << "Info recue: " << user.getIdUser() << ", " << user.getUserName();
    
    ControllerUser Missing method
    _controllerUser.infoUser(user);
}
