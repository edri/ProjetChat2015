#include "serverControllerInput.h"

ServerControllerInput::ServerControllerInput(ControllerUser& controllerUser, ControllerRoom& controllerRoom) : _controllerUser(controllerUser), _controllerRoom(controllerRoom) {}

void ServerControllerInput::receiveMessage(const ModelMessage& message, QObject* sender)
{
    Q_UNUSED(message);
    Q_UNUSED(sender);
}

void ServerControllerInput::login(const QString& pseudo, const QString& hashedPWD, QObject* sender)
{
    ChatorClient* client = (ChatorClient*) sender;
    _controllerUser.login(pseudo, hashedPWD, client);
}
