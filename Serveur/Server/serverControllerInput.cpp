#include "serverControllerInput.h"

ServerControllerInput::ServerControllerInput(ControllerUser& controllerUser, ControllerRoom& controllerRoom) : _controllerUser(controllerUser), _controllerRoom(controllerRoom) {}

void ServerControllerInput::receiveMessage(ModelMessage& message, const bool edited, QObject* sender)
{
    _controllerRoom.processMessage(message, edited, (ChatorClient*) sender);
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
        Q_UNUSED(i);
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

void ServerControllerInput::disconnect(const quint32 userId, QObject* sender)
{
    Q_UNUSED(userId);
    _controllerUser.disconnect((ChatorClient*) sender);
}

void ServerControllerInput::createAccount(ModelUser& user, QObject* sender)
{
    _controllerUser.createAccount(user, (ChatorClient*) sender);
}

void ServerControllerInput::deleteRoom(const quint32 roomId, QObject* sender)
{
    Q_UNUSED(roomId);
    Q_UNUSED(sender);
    // IL FAUT FAIRE QQCH ICI.
}

void ServerControllerInput::leaveRoom(const quint32 roomId, QObject* sender)
{
    Q_UNUSED(roomId);
    Q_UNUSED(sender);
    // IL FAUT FAIRE QQCH ICI.
}
