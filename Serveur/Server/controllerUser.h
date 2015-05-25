#ifndef CONTROLLER_USER_H
#define CONTROLLER_USER_H

#include "controllerDB.h"
#include "serverModel.h"
#include "../Interpretor/interpretor.h"
#include "controllerRoom.h"

class ControllerRoom;

class ControllerUser
{
    public:
    ControllerUser(ControllerDB& db);
    void login(const QString& pseudo, const QString& hashedPWD, ChatorClient* client);
    void userId(const QString& userName, ChatorClient* client);
    void createAccount(ModelUser& user, const QByteArray& password, ChatorClient* client);
    void disconnect(ChatorClient* client);
    void modifyUser(const ModelUser& user, ChatorClient* client);
    void getSalt(const QString& pseudo, ChatorClient* client);
    void getPublicKey(const quint32 idUser, ChatorClient* client);
    
    QMap<quint32, ChatorClient*>& getConnectedUsers();
    Interpretor* _interpretor;
    ControllerRoom* _room;
    
    private:
    ControllerDB& _db;
    QMap<quint32, ChatorClient*> _connectedUsers;
};

#endif
