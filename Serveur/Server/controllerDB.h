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
    bool createAccount(ModelUser& user, QString& password);
    bool userExists(const QString& pseudo, quint32& id);
    ModelUser info(const quint32 id);
    quint32 storeMessage(const ModelMessage& message);
    void editMessage(const ModelMessage& message);
    void deleteMessage(const quint32 id);
    ModelRoom infoRoom(const quint32 id);
    quint32 createRoom(const ModelRoom& room);
    ModelMessage infoMessage(const quint32 id);
    void modifyRoom(const ModelRoom& room);
    void modifyUser(const ModelUser& user);
    void deleteRoom(const quint32 roomId);
    quint64 saveImage(const QImage& image);
    void leaveRoom(const quint32 idUser, const quint32 idRoom);
    
    private :
    QSqlDatabase _db;
};

#endif
