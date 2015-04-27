#include "controllerDB.h"
#include <QtCore/QDebug>
#include <QFile>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

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
    //query.prepare("SELECT idUser FROM user WHERE login = :login AND password = :password");
    //query.bindValue(":login", pseudo);
    //query.bindValue(":password", hashedPWD);
    query.exec("SELECT idUser FROM user WHERE login = \"" + pseudo + "\" AND password = \"" + hashedPWD + "\"");

    if (!query.first()) {return false;}
    
    id = query.record().value(0).toUInt();
    
    query.exec("UPDATE user SET lastConnection = datetime('NOW') WHERE idUser = " + QString::number(id));

    return true;
}

ModelUser ControllerDB::info(const quint32 id)
{
    QSqlQuery query(_db);
    QSet<quint32> rooms;
    
    //query.prepare("SELECT idRoom FROM roomMembership INNER JOIN privilege ON roommembership.idPrivilege = privilege.idPrivilege WHERE idUser = :id");
    query.prepare("SELECT idRoom FROM roomMembership WHERE idUser = " + QString::number(id));
    query.exec();
    
    while(query.next())
    {
        rooms.insert(query.record().value(0).toUInt());
    }
    
    query.exec("SELECT idUser, login, firstName, lastName, lastConnection, profilePicture FROM user WHERE idUser = " + QString::number(id));
    query.first();
    
    ModelUser user(query.record().value("idUser").toUInt(), query.record().value("login").toString(), query.record().value("firstName").toString(), query.record().value("lastName").toString(), true, query.record().value("lastConnection").toDateTime(), QImage(), rooms); // query.record().value("profilePicture").toString()
    
    return user;
}

quint32 ControllerDB::storeMessage(const ModelMessage& message)
{
    QSqlQuery query(_db);
    //query.prepare("INSERT INTO message (contents, date, idUser, idRoom, lastUpdated) VALUES (:content, datetime('NOW'), :idUser, :idRoom, datetime('NOW'))");
    //query.bindValue(":content", message.getContent());
    //query.bindValue(":idUser", message.getIdUser());
    //query.bindValue(":idRoom", message.getIdRoom());
    query.exec("INSERT INTO message (contents, date, idUser, idRoom, lastUpdated) VALUES (\"" + message.getContent()  + "\", datetime('NOW'), " + QString::number(message.getIdUser()) + ", " + QString::number(message.getIdRoom()) + ", datetime('NOW'))");
    return query.lastInsertId().toUInt();
}

ModelRoom ControllerDB::infoRoom(const quint32 id)
{    
    // Récupération des membres et des admins
    QSet<quint32> admins;
    QSet<quint32> users;
    
    QSqlQuery query(_db);
    query.prepare("SELECT idUser, name FROM roomMembership INNER JOIN privilege ON roomMembership.idPrivilege = privilege.idPrivilege WHERE idRoom = " + QString::number(id));
    query.exec();
    
    quint32 idUser;
    
    while(query.next())
    {
        idUser = query.record().value("idUser").toUInt();
        if (query.record().value("name").toString() == "admin") {admins.insert(idUser);}
        users.insert(idUser);
    }
    
    // Récupération des messages
    QMap<quint32, ModelMessage> messages;
    
    query.exec("SELECT idMessage, idRoom, idUser, date, contents FROM message WHERE idRoom = " + QString::number(id));
    
    while(query.next())
    {
        ModelMessage message(query.record().value("idMessage").toUInt(), query.record().value("idRoom").toUInt(), query.record().value("idUser").toUInt(), query.record().value("date").toDateTime(), query.record().value("contents").toString());
        messages.insert(message.getIdMessage(), message);
    }
    
    // Récupération des informations de la salle
    query.prepare("SELECT idRoom, name, private, visible, picture, limitOfStoredMessages FROM room WHERE idRoom = " + QString::number(id));
    query.exec();
    query.first();
    
    // Construction de la salle
    
    ModelRoom room(query.record().value("idRoom").toUInt(), query.record().value("name").toString(), query.record().value("limitOfStoredMessages").toUInt(), query.record().value("private").toBool(), query.record().value("visible").toBool(), QImage(), admins, users, messages);
    
    return room;
}

quint32 ControllerDB::createRoom(const ModelRoom& room)
{
    QSqlQuery query(_db);
    
    query.exec("INSERT INTO room (name, limitOfStoredMessages, private, visible, picture) VALUES (\"" + room.getName() + "\", " + room.getLimit() + ", " + QString::number(room.isPrivate()? 1 : 0) + ", " + QString::number(room.isVisible()? 1 : 0) + ", \"\")");
    
    quint32 idRoom = query.lastInsertId().toUInt();
    
    QSet<quint32> users = room.getUsers();
    QSet<quint32> admins = room.getAdmins();
    
    // VOIR LES PREPARED STATEMENTS
    for (quint32 idAdmin : admins)
    {
        query.exec("INSERT INTO roomMembership (idUser, idRoom, idPrivilege) VALUES (" + QString::number(idAdmin) + ", " + QString::number(idRoom) + ", (SELECT idPrivilege FROM privilege WHERE name = \"admin\"))");
        users.remove(idAdmin);
    }
    
    for (quint32 idUser : users)
    {
        query.exec("INSERT INTO roomMembership (idUser, idRoom, idPrivilege) VALUES (" + QString::number(idUser) + ", " + QString::number(idRoom) + ", (SELECT idPrivilege FROM privilege WHERE name = \"user\"))");
    }
    
    return idRoom;
}

bool ControllerDB::userExists(const QString& pseudo, quint32& id)
{
    QSqlQuery query(_db);
    //query.prepare("SELECT idUser FROM user WHERE login = :login AND password = :password");
    //query.bindValue(":login", pseudo);
    //query.bindValue(":password", hashedPWD);
    query.exec("SELECT idUser FROM user WHERE login = \"" + pseudo + "\"");

    if (!query.first()) {return false;}
    
    id = query.record().value(0).toUInt();
    
    return true;
}
