/*
     * Implements serverControllerInput.h
     * The server side requires the sender parameter. It is casted in
     * a ChatorCLient pointer that is needed by the controllers.
*/

#include "serverControllerInput.h"

// The constructor sets the references to the controllers.
ServerControllerInput::ServerControllerInput(ControllerUser& controllerUser, ControllerRoom& controllerRoom) : _controllerUser(controllerUser), _controllerRoom(controllerRoom) {}

// All the following methods are simple adapter dispatching.

void ServerControllerInput::receiveMessage(ModelMessage& message, const bool edited, QObject* sender)
{
    _controllerRoom.processMessage(message, edited, (ChatorClient*) sender);
}

void ServerControllerInput::deleteMessage(const quint32 roomId, const quint32 messageId, QObject* sender)
{
    _controllerRoom.deleteMessage(roomId, messageId, (ChatorClient*) sender);
}

void ServerControllerInput::login(const QString& pseudo, const QByteArray& hashedPWD, QObject* sender)
{
    ChatorClient* client = (ChatorClient*) sender;
    _controllerUser.login(pseudo, hashedPWD, client);
}

void ServerControllerInput::editAccount(ModelUser& user, const QByteArray& password, const QByteArray& privateKey, QObject* sender)
{
    _controllerUser.modifyUser(user, password, privateKey, (ChatorClient*) sender);
}

void ServerControllerInput::room(ModelRoom& room, bool edited, const QMap<quint32, QByteArray>& usersAndKeys, QObject* sender)
{
    ChatorClient* client = (ChatorClient*) sender;
    
    // We check if the room was edited or if it is a new one and call the right method in the room controller.
    if (edited)
    {
        _controllerRoom.modifyRoom(room, usersAndKeys, client);
    }
    else
    {
        _controllerRoom.createRoom(room, usersAndKeys, client);
    }
}

void ServerControllerInput::userId(const QString& userName, bool exists, quint32 userId, QObject* sender)
{
    Q_UNUSED(exists);
    Q_UNUSED(userId);

    _controllerUser.userId(userName, (ChatorClient*) sender);
}

void ServerControllerInput::createAccount(ModelUser& user, const QByteArray& password, const QByteArray& passwordSalt, const QByteArray& keySalt, const QByteArray& privateKey, const QByteArray& publicKey, QObject* sender)
{
    _controllerUser.createAccount(user, password, passwordSalt, keySalt, privateKey, publicKey, (ChatorClient*) sender);
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

void ServerControllerInput::publicKey(QList<QPair<quint32, QByteArray>>& usersIdAndKey, QObject* sender)
{
    _controllerUser.getPublicKeys(usersIdAndKey, (ChatorClient*) sender);
}

void ServerControllerInput::listRooms(const QList<QPair<quint32, QString>>& publicRooms, const QList<QPair<quint32, QString>>& privateVisibleRooms, QObject* sender)
{
    Q_UNUSED(publicRooms);
    Q_UNUSED(privateVisibleRooms);
    
    _controllerUser.getAvalaibleRooms((ChatorClient*) sender);
}

void ServerControllerInput::request(const quint32 roomId, const ModelUser& user, const QByteArray& key, const bool accepted, QObject* sender)
{
    Q_UNUSED(sender);
    
    _controllerRoom.acceptOrDeny(roomId, user.getIdUser(), key, accepted);
}

void ServerControllerInput::joinRoom(const quint32 roomId, QObject* sender)
{
    _controllerRoom.joinRoom(roomId, (ChatorClient*) sender);
}

void ServerControllerInput::disconnect(const quint32 userId, QObject* sender)
{
    Q_UNUSED(userId);
    
    _controllerUser.disconnect((ChatorClient*) sender);
}
