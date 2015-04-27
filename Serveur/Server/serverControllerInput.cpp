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
    //ChatorClient* client = (ChatorClient*) sender;
    Q_UNUSED(user);
    Q_UNUSED(sender);
}

void ServerControllerInput::room(ModelRoom& room, bool edited, QObject* sender)
{
    ChatorClient* client = (ChatorClient*) sender;
    if (edited)
    {
        int i = 0; // Instruction caca pour combler le vide abyssal de la non implémentation de la modification de salle
    }
    else
    {
        _controllerRoom.createRoom(room, client);
    }
}

void ServerControllerInput::userId(const QString& userName, bool exists, quint32 userId, QObject* sender)
{
    Q_UNUSED(exists);
    Q_UNUSED(userId);
    ChatorClient* client = (ChatorClient*) sender;
    _controllerUser.userId(userName, client);
}
