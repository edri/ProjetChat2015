/*
     * Implements clientControllerInput.h
     * The sender parameter is never used client side, it is
     * a server only requirement.
*/

#include <QtCore/QDebug>
#include <QDataStream>
#include "clientControllerInput.h"
#include <cstring>

// The empty constructor sets empty values for the pointers to the controllers.
ClientControllerInput::ClientControllerInput() : _controllerChat(nullptr), _controllerUser(nullptr), _controllerRoom(nullptr) {}

// This constructor sets the pointers to the controllers.
ClientControllerInput::ClientControllerInput(ControllerChat* controllerChat, ControllerUser* controllerUser, ControllerRoom* controllerRoom) : _controllerChat(controllerChat), _controllerUser(controllerUser), _controllerRoom(controllerRoom) {}

// Setters for the pointers to the controllers so that we can link them after construction.
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

// Other simple adapter dispatching

void ClientControllerInput::receiveMessage(ModelMessage& message, const bool edited, QObject* sender)
{
    Q_UNUSED(sender);

    _controllerChat->receiveMessage(message, edited);
}

void ClientControllerInput::deleteMessage(const quint32 roomId, const quint32 messageId, QObject* sender)
{
    Q_UNUSED(sender);
    
    _controllerChat->deleteMessageInModel(roomId, messageId);
}

void ClientControllerInput::infoUser(ModelUser& user, QByteArray& keySalt, QByteArray& publicKey, QByteArray& privateKey, QObject* sender)
{
    Q_UNUSED(sender);

    // We have to deserialize the keys
    Salt salt;
    RSAPair rsaKeys;
    
    salt.resize(keySalt.size());
    memcpy(salt.data(), keySalt.data(), salt.size());
    
    rsaKeys.privateKey.resize(privateKey.size());
    rsaKeys.publicKey.resize(publicKey.size());
    memcpy(rsaKeys.privateKey.data(), privateKey.data(), rsaKeys.privateKey.size());
    memcpy(rsaKeys.publicKey.data(), publicKey.data(), rsaKeys.publicKey.size());
    
    _controllerUser->infoUser(user, salt, rsaKeys);
}

void ClientControllerInput::join(const QMap<quint32, ModelRoom>& rooms, const QMap<quint32, ModelUser>& users, QObject* sender)
{
    Q_UNUSED(sender);
    
    // We have to store / update every user sent by the server.
    for (ModelUser user : users)
    {
        _controllerChat->loadUser(user);
    }
    
    // We have to store / update every room sent by the server.
    for (ModelRoom room : rooms)
    {
        _controllerChat->loadRoom(room);
    }
    
    // Refresh the interface.
    _controllerChat->loadUserRooms();
}

void ClientControllerInput::userId(const QString& userName, bool exists, quint32 userId, QObject* sender)
{
    Q_UNUSED(sender);
    Q_UNUSED(userName);
    
    // If we are already logged in, this packet has to be sent to the room controller (we are creating or editing a room).
    if (_controllerChat->isControllerActive())
    {
        _controllerRoom->userId(exists, userId);
    }
    
    // The user tries to subscribe and has to know if the chosen login is available, we have to send the packet to the user controller.
    else
    {
        _controllerUser->usernameResponse(exists);
    }
}

void ClientControllerInput::connected(const quint32 userId, QObject *sender)
{
    Q_UNUSED(sender);

    _controllerChat->userStatusChanged(userId, true);
}

void ClientControllerInput::disconnect(const quint32 userId, QObject* sender)
{
    Q_UNUSED(sender);

    _controllerChat->userStatusChanged(userId, false);
}

void ClientControllerInput::deleteRoom(const quint32 roomId, QObject* sender)
{
    Q_UNUSED(sender);

    _controllerChat->deleteRoomInModel(roomId);
}

void ClientControllerInput::leaveRoom(const quint32 userId, const quint32 roomId, QObject *sender)
{
    Q_UNUSED(sender);

    _controllerChat->leaveRoomInModel(userId, roomId);
}

void ClientControllerInput::salt(const QString& pseudo, const QByteArray& salt, QObject* sender)
{
    Q_UNUSED(pseudo);
    Q_UNUSED(sender);
    
    // We have to deserialize the salt
    Salt s;
    s.resize(salt.size());
    memcpy(s.data(), salt.data(), salt.size());
    
    _controllerUser->receiveSalt(s);
}

void ClientControllerInput::publicKey(QList<QPair<quint32, QByteArray>>& usersIdAndKey, QObject* sender)
{
    Q_UNUSED(sender);
    _controllerRoom->createRoom(usersIdAndKey);
}

void ClientControllerInput::listRooms(const QList<QPair<quint32, QString>>& publicRooms, const QList<QPair<quint32, QString>>& privateVisibleRooms, QObject* sender)
{
    Q_UNUSED(sender);
    _controllerRoom->listRooms(publicRooms, privateVisibleRooms);
}

void ClientControllerInput::room(ModelRoom& room, bool edited, const QMap<quint32, QByteArray>& usersAndKeys, QObject* sender)
{
    Q_UNUSED(usersAndKeys);
    Q_UNUSED(sender);

    if (edited)
    {
        _controllerChat->editRoom(room);
    }
}

// This packet is not supported yet.
void ClientControllerInput::editAccount(ModelUser& user, const QByteArray& password, const QByteArray& privateKey, QObject* sender)
{
    Q_UNUSED(user);
    Q_UNUSED(password);
    Q_UNUSED(privateKey);
    Q_UNUSED(sender);
}

void ClientControllerInput::request(const quint32 roomId, const ModelUser& user, const QByteArray& publicKey, const bool accepted, QObject* sender)
{
    Q_UNUSED(accepted);
    Q_UNUSED(sender);

    _controllerChat->newMembershipRequest(roomId, user, publicKey);
}

// The server sent an error
void ClientControllerInput::error(const ModelError& error)
{
    switch(error.getErrorType())
    {
        case ErrorType::AUTH_ERROR:
        {
            _controllerUser->authError();
        }    
        break;
        
        default:
        {
            break;
        }
    }
}
