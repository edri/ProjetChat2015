#include "serverControllerInput.h"

ServerControllerInput::ServerControllerInput(ControllerUser& controllerUser, ControllerRoom& controllerRoom) : _controllerUser(controllerUser), _controllerRoom(controllerRoom) {}

void ServerControllerInput::receiveMessage(ModelMessage& message, const bool edited, QObject* sender)
{
    _controllerRoom.processMessage(message, edited, (ChatorClient*) sender);
}

void ServerControllerInput::deleteMessage(const quint32 roomId, const quint32 messageId, QObject* sender)
{
    _controllerRoom.deleteMessage(roomId, messageId, (ChatorClient*) sender);
}

void ServerControllerInput::login(const QString& pseudo, const QString& hashedPWD, QObject* sender)
{
    ChatorClient* client = (ChatorClient*) sender;
    _controllerUser.login(pseudo, hashedPWD, client);
}

void ServerControllerInput::infoUser(ModelUser& user, QObject* sender)
{
    _controllerUser.modifyUser(user, (ChatorClient*) sender);
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

void ServerControllerInput::createAccount(ModelUser& user, const QByteArray& password, QObject* sender)
{
    qDebug() << "ServerControllerInput CreateAccount";
    _controllerUser.createAccount(user, password, (ChatorClient*) sender);
}

void ServerControllerInput::editAccount(ModelUser& user, QObject* sender)
{
    _controllerUser.modifyUser(user, (ChatorClient*) sender);
}

void ServerControllerInput::deleteRoom(const quint32 roomId, QObject* sender)
{
    _controllerRoom.deleteRoom(roomId, (ChatorClient*) sender);
}

void ServerControllerInput::leaveRoom(const quint32 userId, const quint32 roomId, QObject* sender)
{
    _controllerRoom.leaveRoom(userId, roomId, (ChatorClient*) sender);
}

void ServerControllerInput::salt(const QString& pseudo, const QByteArray& salt, QObject* sender)
{
    Q_UNUSED(salt);
    _controllerUser.getSalt(pseudo, (ChatorClient*) sender);
}

void ServerControllerInput::publicKey(const quint32 idUser, const QByteArray& key, QObject* sender)
{
    Q_UNUSED(key);
    _controllerUser.getPublicKey(idUser, (ChatorClient*) sender);
}
