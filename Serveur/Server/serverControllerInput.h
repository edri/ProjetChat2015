/*
     * Created by Benoist Wolleb
     *
     * Contains methods called everytime the server receive instruction from
     * the client
     *
*/

#ifndef SERVER_CONTROLLER_INPUT_H
#define SERVER_CONTROLLER_INPUT_H

#include "../controllerInput/controllerInput.h"
#include "controllerUser.h"
#include "controllerRoom.h"
#include "serverModel.h"

class ServerControllerInput : public ControllerInput
{
    Q_OBJECT
    
    public:
    ServerControllerInput(ControllerUser& controllerUser, ControllerRoom& controllerRoom);
    
    void createAccount(ModelUser& user, const QByteArray& password, const QByteArray& passwordSalt, const QByteArray& keySalt, const QByteArray& privateKey, const QByteArray& publicKey, QObject* sender);
    void receiveMessage(ModelMessage& message, const bool edited, QObject* sender);
    void deleteMessage(const quint32 roomId, const quint32 messageId, QObject* sender);
    void login(const QString& pseudo, const QByteArray& hashedPWD, QObject* sender);
    void room(ModelRoom& room, bool edited, QList<quint32> usersIds, QList<QPair<QByteArray, QByteArray>> cryptedKeys, QObject* sender);
    void userId(const QString& userName, bool exists, quint32 userId, QObject* sender);
    void disconnect(const quint32 userId, QObject* sender);
    void deleteRoom(const quint32 roomId, QObject* sender);
    void leaveRoom(const quint32 userId, const quint32 roomId, QObject* sender);
    void salt(const QString& pseudo, const QByteArray& salt, QObject* sender);
    void publicKey(QList<QPair<quint32, QByteArray>>& usersIdAndKey, QObject* sender);
    void listRooms(const QList<QPair<quint32, QString>>& publicRooms, const QList<QPair<quint32, QString>>& privateVisibleRooms, QObject* sender);
    void editAccount(ModelUser& user, const QByteArray& password, const QByteArray& privateKey, QObject* sender);
    void request(const quint32 roomId, const ModelUser& user, const QByteArray& key, const bool accepted, QObject* sender);
    
    
    private:
    ControllerUser& _controllerUser;
    ControllerRoom& _controllerRoom;
};

#endif
