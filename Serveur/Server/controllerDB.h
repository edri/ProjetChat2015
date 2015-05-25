#ifndef CONTROLLER_DB_H
#define CONTROLLER_DB_H

#include <QSqlDatabase>
#include <QString>
#include "../../ModeleChator/modelChator.h"
#include "../../ModeleChator/chatorConstants.h"
#include "serverModel.h"

class ControllerDB
{
    public:
    ControllerDB(const QString& dbName = DATABASE_FILE_NAME);
    bool connect();
    bool init();
    bool login(const QString& pseudo, const QString& hashedPWD, quint32& id);
    void logout(const quint32 userId);
    bool createAccount(ModelUser& user, const QByteArray& password, const QByteArray& passwordSalt, const QByteArray& keySalt, const QByteArray& privateKey, const QByteArray& publicKey);
    bool userExists(const QString& pseudo, quint32& id);
    ModelUser info(const quint32 id);
    quint32 storeMessage(const ModelMessage& message);
    void editMessage(const ModelMessage& message);
    void deleteMessage(const quint32 id);
    ModelRoom infoRoom(const quint32 id);
    quint32 createRoom(ModelRoom& room);
    ModelMessage infoMessage(const quint32 id);
    void modifyRoom(const ModelRoom& room);
    void modifyUser(const ModelUser& user);
    void deleteRoom(const quint32 roomId);
    quint64 saveImage(const QImage& image);
    void leaveRoom(const quint32 idUser, const quint32 idRoom);
    QByteArray getSalt(const QString& pseudo);
    QByteArray getPublicKey(const quint32 idUser);
    
    private :
    QSqlDatabase _db;
};

#endif
