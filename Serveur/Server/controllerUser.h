/*
     * Created by Benoist Wolleb
     *
     * Processed requests sent by users and related to users such
     * as login, salt request in order to hash his password.
*/

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
    void login(const QString& pseudo, const QByteArray& hashedPWD, ChatorClient* client);
    void userId(const QString& userName, ChatorClient* client);
    void createAccount(ModelUser& user, const QByteArray& password, const QByteArray& passwordSalt, const QByteArray& keySalt, const QByteArray& privateKey, const QByteArray& publicKey, ChatorClient* client);
    void disconnect(ChatorClient* client);
    void modifyUser(const ModelUser& user, ChatorClient* client);
    void getSalt(const QString& pseudo, ChatorClient* client);
    void getPublicKeys(QList<QPair<quint32, QByteArray>>& usersIdAndKey, ChatorClient* client);
    
    QMap<quint32, ChatorClient*>& getConnectedUsers();
    Interpretor* _interpretor;
    ControllerRoom* _room;
    
    private:
    ControllerDB& _db;
    QMap<quint32, ChatorClient*> _connectedUsers;
};

#endif
