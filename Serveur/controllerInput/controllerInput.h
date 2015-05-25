 #ifndef CONTROLLER_INPUT_H
#define CONTROLLER_INPUT_H

#include <QtCore/QObject>
#include "../../ModeleChator/modelChator.h"

class ControllerInput : public QObject
{
    Q_OBJECT
    
    public:
    virtual void createAccount(ModelUser& user, const QByteArray& password, QObject* sender) {Q_UNUSED(user); Q_UNUSED(password); Q_UNUSED(sender);}
    virtual void editAccount(ModelUser& user, QObject* sender) {Q_UNUSED(user); Q_UNUSED(sender);}
    virtual void receiveMessage(ModelMessage& message, const bool edited, QObject* sender) = 0;
    virtual void deleteMessage(const quint32 roomId, const quint32 messageId, QObject* sender) = 0;
    virtual void connected(const quint32 userId, QObject* sender) { Q_UNUSED(userId); Q_UNUSED(sender) }
    virtual void disconnect(const quint32 userId, QObject* sender) = 0;
    virtual void login(const QString& pseudo, const QString& hashedPWD, QObject* sender) {Q_UNUSED(pseudo); Q_UNUSED(hashedPWD); Q_UNUSED(sender);}
    virtual void salt(const QString& pseudo, const QByteArray& salt, QObject* sender) = 0;
    virtual void publicKey(const quint32 idUser, const QByteArray& key, QObject* sender) = 0;
    virtual void infoUser(ModelUser& user, QObject* sender) = 0;
    virtual void room(ModelRoom& room, bool edited, QObject* sender) {Q_UNUSED(room); Q_UNUSED(edited); Q_UNUSED(sender);}
    virtual void join(const QMap<quint32, ModelRoom>& rooms, const QMap<quint32, ModelUser>& users, QObject* sender) {Q_UNUSED(rooms); Q_UNUSED(users); Q_UNUSED(sender);}
    virtual void userId(const QString& userName, bool exists, quint32 userId, QObject* sender) {Q_UNUSED(userName); Q_UNUSED(exists); Q_UNUSED(userId); Q_UNUSED(sender);}
    virtual void deleteRoom(const quint32 roomId, QObject* sender) = 0;
    virtual void leaveRoom(const quint32 userId, const quint32 roomId, QObject* sender) = 0;
};

#endif
