#include "controllerOutput.h"
#include "../../ModeleChator/modelChator.h"

ControllerOutput::ControllerOutput(Connector& connector, Interpretor& interpretor) : _connector(connector), _interpretor(interpretor) {};

void ControllerOutput::sendMessage(const ModelMessage& message, const bool edited)
{
    _connector.send(_interpretor.sendMessage(message, edited));
}

void ControllerOutput::deleteMessage(const quint32 messageId)
{
    _connector.send(_interpretor.deleteMessage(messageId));
}

void ControllerOutput::login(const QString& pseudo, const QString& hashedPwd)
{
    _connector.send(_interpretor.login(pseudo, hashedPwd));
}

void ControllerOutput::createAccount(const ModelUser& user)
{
    _connector.send(_interpretor.createAccount(user));
}

void ControllerOutput::sendInfoUser(const ModelUser& user)
{
    _connector.send(_interpretor.sendInfoUser(user));
}


void ControllerOutput::join(const quint32 idUser, const quint32 idRoom)
{
    //_connector.send(_interpretor.join(idUser, idRoom));
}

void ControllerOutput::leave(const quint32 idUser, const quint32 idRoom)
{
    _connector.send(_interpretor.leave(idUser, idRoom));
}

void ControllerOutput::disconnect(const quint32 idUser)
{
    _connector.send(_interpretor.disconnect(idUser));
}

void ControllerOutput::room(const ModelRoom& room, bool edited)
{
    _connector.send(_interpretor.room(room, edited));
}

void ControllerOutput::userId(const QString& userName, bool exists, quint32 userId)
{
    _connector.send(_interpretor.userId(userName, exists, userId));
}

void ControllerOutput::deleteRoom(const quint32 roomId)
{
    _connector.send(_interpretor.deleteRoom(roomId));
}

void ControllerOutput::leaveRoom(const quint32 roomId)
{
    _connector.send(_interpretor.leaveRoom(roomId));
}
