#include "controllerDB.h"
#include <QtCore/QDebug>
#include <QFile>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

ControllerDB::ControllerDB(const QString& dbName)
{
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(dbName);
    
    if (! connect())
    {
        qDebug() << "Unable to connect to or initialize the database, there's noting to do...";
    }
}

bool ControllerDB::connect()
{
    bool existingDatabase = QFile::exists(_db.databaseName());
    
    if (! _db.open())
    {
        qDebug() << "Could not connect to database: " << _db.lastError().text();
        return false;
    }
    
    if (!existingDatabase)
    {
        qDebug() << "There is no database, creating a fresh one...";
        return init();
    }
    
    return true;
}

bool ControllerDB::init()
{
    QFile initScript("init.sql");
    if (! initScript.open(QIODevice::ReadOnly))
    {
        qDebug() << "No init script could be found or we cannot read it";
        return false;
    }
    
    QStringList queries = QString::fromUtf8(initScript.readAll()).split(";");
    initScript.close();
    QSqlQuery sqlQuery(_db);
    
    for (QString query : queries)
    {
        query = query.trimmed();
        if (query.isEmpty()) {continue;}
        if (!sqlQuery.exec(query))
        {
            qDebug() << "Database initialization failed: " << _db.lastError().text();
            return false;
        }
    }
    
    return true;
}

bool ControllerDB::login(const QString& pseudo, const QString& hashedPWD, quint32& id)
{
    QSqlQuery query(_db);
    query.prepare("SELECT idUser FROM user WHERE login = :login AND password = :password");
    query.bindValue(":login", pseudo);
    query.bindValue(":password", hashedPWD);
    query.exec();
    
    if (query.size() != 1) {return false;}
    
    query.first();
    id = query.record().value(0).toUInt();
    
    query.prepare("UPDATE user SET lastConnection = datetime('NOW') WHERE idUser = :id");
    query.bindValue(":id", id);
    query.exec()

    return true;
}

ModelUser ControllerDB::info(const quint32 id)
{
    QSqlQuery query(_db);
    query.prepare("SELECT idUser, login, firstName, lastName, lastConnection, profilePicture FROM user WHERE idUser = :id");
    query.bindValue(":id", id);
    query.exec();
    query.first();
    
    ModelUser user(query.record().value("idUser").toUInt(), query.record().value("login").toString(), query.record().value("firstName").toString(), query.record().value("lastName").toString(), true, query.record().value("lastConnection").toDateTime(), QImage()); // query.record().value("profilePicture").toString()
    QSet<quint32> rooms;
    
    //query.prepare("SELECT idRoom FROM roomMembership INNER JOIN privilege ON roommembership.idPrivilege = privilege.idPrivilege WHERE idUser = :id");
    query.prepare("SELECT idRoom FROM roomMembership WHERE idUser = :id");
    query.bindValue(":id", id);
    query.exec();
    
    while(query.next())
    {
        rooms.insert(query.record().value(0).toUInt());
    }
    
    return user;
}

quint32 ControllerDB::storeMessage(const ModelMessage& message)
{
    QSqlQuery query(_db);
    query.prepare("INSERT INTO message (contents, date, idUser, idRoom, lastUpdated) VALUES (:content, datetime('NOW'), :idUser, :idRoom, datetime('NOW'))");
    query.bindValue(":content", message.getContent());
    query.bindValue(":idUser", message.getIdUser());
    query.bindValue(":idRoom", message.getIdRoom());
    query.exec();
    return query.lastInsertId().toUInt();
}

ModelRoom ControllerDB::infoRoom(const quint32 id)
{
    QSqlQuery query(_db);
    query.prepare("SELECT idRoom, name, private, visible, picture, limitOfStoredMessages FROM room WHERE idRoom = :id");
    query.bindValue(":id", id);
    query.exec();
    query.first();
    
    ModelRoom room(query.record().value("idRoom").toUInt(), query.record().value("name").toString(), query.record().value("private").toBool(), query.record().value("visible").toBool(), QImage(), query.record().value("limitOfStoredMessages").toUInt());
    
    QSet<quint32> admins;
    QSet<quint32> users;
    
    QSqlQuery query(_db);
    query.prepare("SELECT idUser, name FROM roomMembership INNER JOIN privilege ON roomMembership.idPrivilege = privilege.idPrivilege WHERE idRoom = :id");
    query.bindValue(":id", id);
    query.exec();
    
    while(query.next())
    {
        if (query.record().value("name").toString() == "admin") {admins.insert(query.record().value("idUser").toUInt());}
        else if (query.record().value("name").toString() == "user") {users.insert(query.record().value("idUser").toUInt());}
    }
    
    QMap<quint32, ModelMessage> messages;
    
    QSqlQuery query(_db);
    query.prepare("SELECT idMessage, idRoom, idUser, date, contents FROM message WHERE idRoom = :id");
    query.bindValue(":id", id);
    query.exec();
    
    while(query.next())
    {
        ModelMessage message(query.record().value("idMessage").toUInt(), query.record().value("idRoom").toUInt(), query.record().value("idUser").toUInt(), query.record().value("date").toDateTime(), query.record().value("content").toString());
        messages.insert(message.getIdMessage(), message);
    }
    
    return room;
}
