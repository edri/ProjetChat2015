/*
     * Created by Benoist Wolleb
     *
     * Abstract method which contains every methods a programm should
     * implements in order to receive informations from distant sender.
*/

#ifndef CONTROLLER_INPUT_H
#define CONTROLLER_INPUT_H

#include <QtCore/QObject>
#include "../../ModeleChator/modelChator.h"

class ControllerInput : public QObject
{
    Q_OBJECT
    
    public:
    virtual void createAccount(ModelUser& user, const QByteArray& password, const QByteArray& passwordSalt, const QByteArray& keySalt, const QByteArray& privateKey, const QByteArray& publicKey, QObject* sender) {Q_UNUSED(user); Q_UNUSED(password); Q_UNUSED(passwordSalt); Q_UNUSED(keySalt); Q_UNUSED(privateKey); Q_UNUSED(publicKey); Q_UNUSED(sender);}
    virtual void editAccount(ModelUser& user, const QByteArray& password, const QByteArray& privateKey, QObject* sender) = 0;
    virtual void receiveMessage(ModelMessage& message, const bool edited, QObject* sender) = 0;
    virtual void deleteMessage(const quint32 roomId, const quint32 messageId, QObject* sender) = 0;
    virtual void connected(const quint32 userId, QObject* sender) { Q_UNUSED(userId); Q_UNUSED(sender) }
    virtual void disconnect(const quint32 userId, QObject* sender) = 0;
    virtual void login(const QString& pseudo, const QByteArray& hashedPWD, QObject* sender) {Q_UNUSED(pseudo); Q_UNUSED(hashedPWD); Q_UNUSED(sender);}
    virtual void salt(const QString& pseudo, const QByteArray& salt, QObject* sender) = 0;
    virtual void publicKey(QList<QPair<quint32, QByteArray>>& usersIdAndKey, QObject* sender) {Q_UNUSED(usersIdAndKey); Q_UNUSED(sender);}
    virtual void infoUser(ModelUser& user, QByteArray& keySalt, QByteArray& publicKey, QByteArray& privateKey, QObject* sender) {Q_UNUSED(user); Q_UNUSED(keySalt); Q_UNUSED(publicKey); Q_UNUSED(privateKey); Q_UNUSED(sender);};
    virtual void room(ModelRoom& room, bool edited, const QMap<quint32, QByteArray>& usersAndKeys, QObject* sender) = 0;
    virtual void join(const QMap<quint32, ModelRoom>& rooms, const QMap<quint32, ModelUser>& users, QObject* sender) {Q_UNUSED(rooms); Q_UNUSED(users); Q_UNUSED(sender);}
    virtual void userId(const QString& userName, bool exists, quint32 userId, QObject* sender) {Q_UNUSED(userName); Q_UNUSED(exists); Q_UNUSED(userId); Q_UNUSED(sender);}
    virtual void deleteRoom(const quint32 roomId, QObject* sender) = 0;
    virtual void leaveRoom(const quint32 userId, const quint32 roomId, QObject* sender) = 0;
    virtual void listRooms(const QList<QPair<quint32, QString>>& publicRooms, const QList<QPair<quint32, QString>>& privateVisibleRooms, QObject* sender) = 0;
    virtual void request(const quint32 roomId, const ModelUser& user, const QByteArray& publicKey, const bool accepted, QObject* sender) = 0;
    virtual void joinRoom(const quint32 roomId, QObject* sender) {Q_UNUSED(roomId); Q_UNUSED(sender);}
};

#endif
