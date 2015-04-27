#include "serverControllerInput.h"

ServerControllerInput::ServerControllerInput(ControllerUser& controllerUser, ControllerRoom& controllerRoom) : _controllerUser(controllerUser), _controllerRoom(controllerRoom) {}

void ServerControllerInput::receiveMessage(ModelMessage& message, QObject* sender)
{
    ChatorClient* client = (ChatorClient*) sender;
    _controllerRoom.storeMessage(message, client);
}

void ServerControllerInput::login(const QString& pseudo, const QString& hashedPWD, QObject* sender)
{
    ChatorClient* client = (ChatorClient*) sender;
    _controllerUser.login(pseudo, hashedPWD, client);
}

void ServerControllerInput::infoUser(ModelUser& user, QObject* sender)
{
    ChatorClient* client = (ChatorClient*) sender;
}

void ServerControllerInput::room(const ModelRoom& room, bool edited, QObject* sender)
{
    Q_UNUSED(room);
    Q_UNUSED(sender);
    Q_UNUSED(edited);
}
