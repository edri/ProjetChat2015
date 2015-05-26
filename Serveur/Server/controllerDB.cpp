/*
     * Created by Benoist Wolleb
     *
     * Implements controllerDB.h
*/

#include "controllerDB.h"
#include <QtCore/QDebug>
#include <QFile>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QFileInfo>

ControllerDB::ControllerDB(const QString& dbName)
{
    _db = QSqlDatabase::addDatabase(DATABASE_TYPE);
    _db.setDatabaseName(dbName);
    
    if (! connect())
    {
        qDebug() << "Unable to connect to or initialize the database, there's noting to do...";
        exit(EXIT_FAILURE);
    }
    
    QFileInfo profilePicturesInfo(PROFILE_PICTURE_FOLDER);
    QDir profilePicturesDirectory;
    
    if ((!profilePicturesInfo.exists() && !profilePicturesDirectory.mkdir(PROFILE_PICTURE_FOLDER)) || !profilePicturesInfo.isDir() || !profilePicturesInfo.isReadable() || !profilePicturesInfo.isWritable())
    {
        qDebug() << "Cannot access the picture folder properly: " << PROFILE_PICTURE_FOLDER;
        exit(EXIT_FAILURE);
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
    QFile initScript(DATABASE_INIT_SCRIPT);
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

bool ControllerDB::login(const QString& pseudo, const QByteArray& hashedPWD, quint32& id)
{    
    QSqlQuery query(_db);
    query.prepare("SELECT idUser FROM user WHERE login = :login AND password = :password");
    query.bindValue(":login", pseudo);
    query.bindValue(":password", hashedPWD);
	query.exec();

    if (!query.first()) {return false;}
    
    id = query.record().value(0).toUInt();

	query.prepare("UPDATE user SET lastConnection = datetime('NOW'), isConnected = 1 WHERE idUser = :idUser");
	query.bindValue(":idUser", id);
	query.exec();

    return true;
}

ModelUser ControllerDB::info(const quint32 id)
{
    QSqlQuery query(_db);
    QSet<quint32> rooms;
    
    query.prepare("SELECT idRoom FROM roomMembership INNER JOIN privilege ON roommembership.idPrivilege = privilege.idPrivilege WHERE idUser = :id");
	query.bindValue(":id", id);
	query.exec();
    
    while(query.next())
    {
        rooms.insert(query.record().value(0).toUInt());
    }
    
    query.prepare("SELECT idUser, login, firstName, lastName, lastConnection, profilePicture, isConnected FROM user WHERE idUser = :id");
	query.bindValue(":id", id);
	query.exec();

    query.first();
    
    ModelUser user(query.record().value("idUser").toUInt(), query.record().value("login").toString(), query.record().value("firstName").toString(), query.record().value("lastName").toString(), query.record().value("isConnected").toBool(), query.record().value("lastConnection").toDateTime(), QImage(query.record().value("profilePicture").toString()), rooms);
    
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

void ControllerDB::editMessage(const ModelMessage& message)
{
    QSqlQuery query(_db);
	query.prepare("UPDATE message SET contents = :content, lastUpdated = datetime('NOW') WHERE idMessage = :idMessage");
	query.bindValue(":content", message.getContent());
	query.bindValue(":idMessage", message.getIdMessage());
    query.exec();
}

void ControllerDB::deleteMessage(const quint32 id)
{
    QSqlQuery query(_db);
	query.prepare("DELETE FROM message WHERE idMessage = :idMessage");
	query.bindValue(":idMessage", id);
	query.exec();
}

ModelMessage ControllerDB::infoMessage(const quint32 id)
{
    QSqlQuery query(_db);
	query.prepare("SELECT contents, date, idUser, idRoom, lastUpdated FROM message WHERE idMessage = :idMessage");
	query.bindValue(":idMessage", id);
	query.exec();

    query.first();
    return ModelMessage(id, query.record().value("idRoom").toUInt(), query.record().value("idUser").toUInt(), query.record().value("date").toDateTime(), query.record().value("lastUpdated").toDateTime(), query.record().value("contents").toByteArray());
}

ModelRoom ControllerDB::infoRoom(const quint32 id)
{    
    // Récupération des membres et des admins
    QSet<quint32> admins;
    QSet<quint32> users;
    
    QSqlQuery query(_db);
	query.prepare("SELECT idUser, name FROM roomMembership INNER JOIN privilege ON roomMembership.idPrivilege = privilege.idPrivilege WHERE idRoom = :idRoom");
	query.bindValue(":idRoom", id);
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
	
	query.prepare("SELECT idMessage, idRoom, idUser, date, lastUpdated, contents FROM message WHERE idRoom = :idRoom ORDER BY date desc LIMIT " + QString::number(NB_MESSAGES_TO_FETCH));
	query.bindValue(":idRoom", id);
	query.exec();
    
    while(query.next())
    {
        ModelMessage message(query.record().value("idMessage").toUInt(), query.record().value("idRoom").toUInt(), query.record().value("idUser").toUInt(), query.record().value("date").toDateTime(), query.record().value("lastUpdated").toDateTime(), query.record().value("contents").toByteArray());
        messages.insert(message.getIdMessage(), message);
    }
    
    // Récupération des informations de la salle
    query.prepare("SELECT idRoom, name, private, visible, picture, limitOfStoredMessages FROM room WHERE idRoom = :idRoom");
	query.bindValue(":idRoom", + id);
    query.exec();
    query.first();
    
    // Construction de la salle
    
    ModelRoom room(query.record().value("idRoom").toUInt(), query.record().value("name").toString(), query.record().value("limitOfStoredMessages").toUInt(), query.record().value("private").toBool(), query.record().value("visible").toBool(), QImage(query.record().value("picture").toString()), admins, users, messages);
    
    return room;
}

quint32 ControllerDB::createRoom(ModelRoom& room)
{
    if (room.getPicture().isNull()) {room.setPicture(QImage(DEFAULT_ROOM_PICTURE));}
    
    QSqlQuery query(_db);
	query.prepare("INSERT INTO room (name, limitOfStoredMessages, private, visible, picture) VALUES (:nameRoom, :limitRoom, :isPrivate, :isVisible, :picture)");
    query.bindValue(":nameRoom", room.getName());
	query.bindValue(":limitRoom", room.getLimit());
	query.bindValue(":isPrivate", QString::number(room.isPrivate()? 1 : 0));
	query.bindValue(":isVisible", QString::number(room.isVisible()? 1 : 0));
	query.bindValue(":picture", saveImage(room.getPicture()));
	query.exec();
    
    quint32 idRoom = query.lastInsertId().toUInt();
    
    QSet<quint32> users = room.getUsers();
    QSet<quint32> admins = room.getAdmins();
    
    for (quint32 idAdmin : admins)
    {
		
		query.prepare("INSERT INTO roomMembership (idUser, idRoom, idPrivilege) VALUES (:idAdmin, :idRoom, (SELECT idPrivilege FROM privilege WHERE name = 'admin'))");
		query.bindValue(":idAdmin", idAdmin);
		query.bindValue(":idRoom", idRoom);
		query.exec();
        
        users.remove(idAdmin);
    }
    
    for (quint32 idUser : users)
    {
		query.prepare("INSERT INTO roomMembership (idUser, idRoom, idPrivilege) VALUES (:idUser, :idRoom, (SELECT idPrivilege FROM privilege WHERE name = 'user'))");
        query.bindValue(":idUser", idUser);
		query.bindValue(":idRoom", idRoom);
		query.exec(); 
    }
    
    return idRoom;
}

bool ControllerDB::userExists(const QString& pseudo, quint32& id)
{
    QSqlQuery query(_db);
    query.prepare("SELECT idUser FROM user WHERE login = :login");
    query.bindValue(":login", pseudo);
	query.exec();

    if (!query.first()) {return false;}
    
    id = query.record().value(0).toUInt();
    
    return true;
}

bool ControllerDB::createAccount(ModelUser& user, const QByteArray& password, const QByteArray& passwordSalt, const QByteArray& keySalt, const QByteArray& privateKey, const QByteArray& publicKey)
{
    if (user.getImage().isNull()) {user.setImage(QImage(PROFILE_PICTURE_FOLDER + DEFAULT_PROFILE_PICTURE));}
    quint64 msecs = saveImage(user.getImage());
    
    quint32 id;
    if (userExists(user.getUserName(), id)) {return  false;}
    
    QSqlQuery query(_db);
    
	query.prepare("INSERT INTO user (login, firstName, lastName, password, profilePicture, isConnected, publicKey, privateKey, saltPassword, saltKey) VALUES (:userName, :userFirstName, :userLastName, :password, :msecs, 0, :pubKey, :privKey, :pwdSalt, :keySalt)");
	query.bindValue(":userName", user.getUserName());
	query.bindValue(":userFirstName", user.getFirstName());
	query.bindValue(":userLastName", user.getLastName());
	query.bindValue(":password", password);
	query.bindValue(":msecs", msecs);
	query.bindValue(":pubKey", publicKey);
	query.bindValue(":privKey", privateKey);
	query.bindValue(":pwdSalt", passwordSalt);
	query.bindValue(":keySalt", keySalt);
	query.exec();
	 
    user.setIdUser(query.lastInsertId().toUInt());
    
    qDebug() << "Inscription faite dans la db: " << query.lastError().text();
    
    return true;
}

quint64 ControllerDB::saveImage(const QImage& image)
{
    QFile picture;
    quint64 msecs = QDateTime::currentDateTime().toMSecsSinceEpoch();
    
    do
    {
        picture.setFileName(PROFILE_PICTURE_FOLDER + QString::number(msecs));
    } while (picture.exists() && msecs++);
    
    picture.open(QIODevice::WriteOnly);
    image.save(&picture, PROFILE_PICTURE_FORMAT);
    picture.close();
    
    return msecs;
}

void ControllerDB::logout(const quint32 userId)
{
    QSqlQuery query(_db);
    query.prepare("UPDATE user SET isConnected = 0 WHERE idUser = :idUser");
    query.bindValue(":idUser", userId);
    query.exec();
}

void ControllerDB::modifyUser(const ModelUser& user)
{
    QSqlQuery query(_db);
	query.prepare("UPDATE user (login, firstName, lastName, password, isConnected, publicKey, privateKey, salt, masterKey) SET VALUES (:userName, :userFirstName, :userLastName, password, 1, 0, 0, 0, 0) WHERE idUser = :idUser");
	query.bindValue(":userName", user.getUserName());
	query.bindValue(":userFirstName", user.getFirstName());
	query.bindValue(":userLastName", user.getLastName());
	query.bindValue(":idUser", user.getIdUser());
	query.exec();
	
	query.prepare("SELECT profilePicture FROM user WHERE idUser = :idUser");	
	query.bindValue(":idUser", user.getIdUser());
	query.exec();

    query.first();
    
    QFile profilePicture(query.record().value("profilePicture").toString());
    profilePicture.open(QIODevice::WriteOnly);
    user.getImage().save(&profilePicture, PROFILE_PICTURE_FORMAT);
    profilePicture.close();
}

// A FAIRE AVEC LES PREPARED
void ControllerDB::modifyRoom(const ModelRoom& room)
{
    QSqlQuery query(_db);
    query.exec("UPDATE room (name, limitOfStoredMessages) VALUES (\"" + room.getName() + "\", " + QString::number(room.getLimit()) + ")");
    
    query.exec("SELECT picture FROM room WHERE idRoom = " + QString::number(room.getIdRoom()));
    query.first();
    
    QFile profilePicture(query.record().value("picture").toString());
    profilePicture.open(QIODevice::WriteOnly);
    room.getPicture().save(&profilePicture, PROFILE_PICTURE_FORMAT);
    profilePicture.close();
}

void ControllerDB::deleteRoom(const quint32 roomId)
{
    QSqlQuery query(_db);
	query.prepare("DELETE FROM message WHERE idRoom = :roomId");
	query.bindValue(":roomId", roomId);
	query.exec();
	
	query.prepare("DELETE FROM roomMembership WHERE idRoom = :roomId");
	query.bindValue(":roomId", roomId);
	query.exec();
	
	query.prepare("DELETE FROM banning WHERE idRoom = :idRoom");
	query.bindValue(":idRoom", roomId);
	query.exec();
	
	query.prepare("DELETE FROM room WHERE idRoom = :idRoom");
	query.bindValue(":idRoom", roomId);
	query.exec();
}

// A FAIRE AVEC LES PREPARED
void ControllerDB::leaveRoom(const quint32 idUser, const quint32 idRoom)
{
    QSqlQuery query(_db);
    query.exec("DELETE FROM roomMembership WHERE idUser = " + QString::number(idUser) + " AND idRoom = " + QString::number(idRoom));
    query.exec("SELECT COUNT(idUser) AS nbMembers FROM roomMembership WHERE idRoom = " + QString::number(idRoom));
    query.first();
    if (query.record().value("nbMembers").toUInt() == 0) {deleteRoom(idRoom);}
}

QByteArray ControllerDB::getSalt(const QString& pseudo)
{
    quint32 id;
    if (!userExists(pseudo, id)) {qDebug() << "Non existant"; return QByteArray();}
    
    QSqlQuery query(_db);
    query.prepare("SELECT saltPassword FROM user WHERE idUser = :idUser");
    query.bindValue(":idUser", id);
    query.exec();
    
    query.first();
    return query.record().value("saltPassword").toByteArray();
}

QByteArray ControllerDB::getPublicKey(const quint32 idUser)
{
    QSqlQuery query(_db);
    query.prepare("SELECT publicKey FROM user WHERE idUser = :idUser");
    query.bindValue(":idUser", idUser);
    query.exec();
    
    query.first();
    return query.record().value("publicKey").toByteArray();
}

void ControllerDB::requestAccess(const quint32 idUser, const quint32 idRoom)
{
    QSqlQuery query(_db);
    query.prepare("INSERT INTO roomMembership (idUser, idRoom, idPrivilege) VALUES (:idUser, :idRoom, (SELECT idPrivilege FROM privilege WHERE name = request))");
    query.bindValue(":idUser", idUser);
    query.bindValue(":idRoom", idRoom);
    query.exec();
}

void ControllerDB::setKey(const quint32 idUser, const quint32 idRoom, const QByteArray& aesKey)
{
    QSqlQuery query(_db);
    query.exec("UPDATE roomMembership SET roomKey = :key WHERE idUser = :idUser AND idRoom = :idRoom");
    query.bindValue(":key", aesKey);
    query.bindValue(":idUser", idUser);
    query.bindValue(":idRoom", idRoom);
    query.exec();
}
