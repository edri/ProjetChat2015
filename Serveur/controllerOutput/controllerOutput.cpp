/*
     * Created by Benoist Wolleb
     *
     * Implements controllerOutput.h
*/

#include "controllerOutput.h"
#include "../../ModeleChator/modelChator.h"

ControllerOutput::ControllerOutput(Connector& connector, Interpretor& interpretor) : _connector(connector), _interpretor(interpretor) {};

void ControllerOutput::sendMessage(const ModelMessage& message, const bool edited)
{
    _connector.send(_interpretor.sendMessage(message, edited));
}

void ControllerOutput::deleteMessage(const quint32 roomId, const quint32 messageId)
{
    _connector.send(_interpretor.deleteMessage(roomId, messageId));
}

void ControllerOutput::login(const QString& pseudo, const Hash& hashedPwd)
{
    _connector.send(_interpretor.login(pseudo, hashedPwd));
}

void ControllerOutput::createAccount(const ModelUser& user, const Hash& password, const Salt& passwordSalt, const Salt& keySalt, const RSAPair& asymKeys)
{
    _connector.send(_interpretor.createAccount(user, password, passwordSalt, keySalt, asymKeys));
}

void ControllerOutput::sendInfoUser(const ModelUser& user)
{
    _connector.send(_interpretor.sendInfoUser(user));
}

void ControllerOutput::leave(const quint32 idUser, const quint32 idRoom)
{
    _connector.send(_interpretor.leave(idUser, idRoom));
}

void ControllerOutput::disconnect(const quint32 idUser)
{
    _connector.send(_interpretor.disconnect(idUser));
}

void ControllerOutput::room(const ModelRoom& room, const QMap<quint32, QByteArray>& usersAndKeys, bool edited)
{
    _connector.send(_interpretor.room(room, usersAndKeys, edited));
}

void ControllerOutput::userId(const QString& userName, bool exists, quint32 userId)
{
    _connector.send(_interpretor.userId(userName, exists, userId));
}

void ControllerOutput::deleteRoom(const quint32 roomId)
{
    _connector.send(_interpretor.deleteRoom(roomId));
}

//void ControllerOutput::leaveRoom(const quint32 userId, const quint32 roomId)
//{
    //_connector.send(_interpretor.leaveRoom(roomId));
    //_connector.send(_interpretor.leave(userId, roomId));
//}

void ControllerOutput::editAccount(const ModelUser& user)
{
    _connector.send(_interpretor.editAccount(user));
}

void ControllerOutput::publicKey(const QList<QPair<quint32, QByteArray>>& usersIdAndKey)
{
    _connector.send(_interpretor.publicKey(usersIdAndKey));
}

void ControllerOutput::askForSalt(const QString& pseudo)
{
    _connector.send(_interpretor.salt(pseudo));
}

void ControllerOutput::askForRooms()
{
    _connector.send(_interpretor.listRooms());
}

void ControllerOutput::joinRoom(const quint32 roomId)
{
    _connector.send(_interpretor.joinRoom(roomId));
}

void ControllerOutput::changeRequestStatus(const quint32 roomId, const ModelUser& user,
                                           const QByteArray& publicKey, const bool accepted)
{
    _connector.send(_interpretor.request(roomId, user, publicKey, accepted));
}
