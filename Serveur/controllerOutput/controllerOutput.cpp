#include "controllerOutput.h"
#include "modelChator.h"

ControllerOutput::ControllerOutput(Connector& connector, Interpretor& interpretor) : _connector(connector), _interpretor(interpretor) {};

void ControllerOutput::sendMessage(const ModelMessage& message)
{
    _connector.send(_interpretor.sendMessage(message));
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
    _connector.send(_interpretor.join(idUser, idRoom));
}

void ControllerOutput::leave(const quint32 idUser, const quint32 idRoom)
{
    _connector.send(_interpretor.leave(idUser, idRoom));
}

void ControllerOutput::disconnect(const quint32 idUser)
{
    _connector.send(_interpretor.disconnect(idUser));
}