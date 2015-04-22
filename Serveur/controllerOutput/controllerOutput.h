#ifndef CONTROLLER_OUTPUT_H
#define CONTROLLER_OUTPUT_H

#include <QtCore/QObject>
#include "../Interpretor/interpretor.h"

class ControllerOutput
{
    public:
    ControllerOutput(Connector& connector, Interpretor& interpretor);
    
    void sendMessage(const ModelMessage& message);
    void login(const QString& pseudo, const QString& hashedPwd);
    void createAccount(const ModelUser& user);
    void sendInfoUser(const ModelUser& user);
    void join(const quint32 idUser, const quint32 idRoom);
    void leave(const quint32 idUser, const quint32 idRoom);
    void disconnect(const quint32 idUser);
    
    private:
    Connector& _connector;
    Interpretor& _interpretor;
};

#endif
