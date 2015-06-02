/*
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
    // Creation of the database
    _db = QSqlDatabase::addDatabase(DATABASE_TYPE);
    _db.setDatabaseName(dbName);
    
    // We try to connect
    if (! connect())
    {
        qDebug() << "Unable to connect to or initialize the database, there's nothing to do...";
        exit(EXIT_FAILURE);
    }
    
    // We check if we can access and write into the picture folder
    QFileInfo profilePicturesInfo(PROFILE_PICTURE_FOLDER);
    QDir profilePicturesDirectory;
    
    if ((!profilePicturesInfo.exists() && !profilePicturesDirectory.mkdir(PROFILE_PICTURE_FOLDER)) || !profilePicturesInfo.isDir() || !profilePicturesInfo.isReadable() || !profilePicturesInfo.isWritable())
    {
        qDebug() << "Cannot access the picture folder properly: " << PROFILE_PICTURE_FOLDER;
        exit(EXIT_FAILURE);
    }
    
    
    // This timer will have to regularily clean the database, so we connect its signal
    QObject::connect(&_timer, SIGNAL(timeout()), this, SLOT(cleanDatabase()));
    
    // We try to get the last clean date
    QSqlQuery query(_db);
    query.exec("SELECT strftime('%s','now') - strftime('%s', MAX(dateCleaning)) AS nbSeconds FROM cleaning");
    query.first();
    int difference = query.record().value("nbSeconds").toUInt();
    
    // If the difference between the last clean date and now is too important, we have to clean the database now
    if (difference >= CLEANING_INTERVAL)
    {
        qDebug() << "It has been a long time since last cleaning, let's do it now" << difference;
        cleanDatabase();
    }
    else
    {
        // We set the timer and start it
        _timer.setInterval((CLEANING_INTERVAL - difference) * 1000);
        _timer.start();
    }
    
}

bool ControllerDB::connect()
{
    
    bool existingDatabase = QFile::exists(_db.databaseName());
    
    // Can we open the database?
    if (! _db.open())
    {
        qDebug() << "Could not connect to database: " << _db.lastError().text();
        return false;
    }
    
    // If the sqlite doesn't exists, we have to build a new one
    if (!existingDatabase)
    {
        qDebug() << "There is no database, creating a fresh one...";
        return init();
    }
    
    return true;
}

bool ControllerDB::init()
{
    // We have to read the initscript
    QFile initScript(DATABASE_INIT_SCRIPT);
    if (! initScript.open(QIODevice::ReadOnly))
    {
        qDebug() << "No init script could be found or we cannot read it";
        return false;
    }
    
    // Load every queries in this script
    QStringList queries = QString::fromUtf8(initScript.readAll()).split(";");
    initScript.close();
    QSqlQuery sqlQuery(_db);
    
    // Execute every query
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
    query.prepare("SELECT idUser, isConnected FROM user WHERE login = :login AND password = :password");
    query.bindValue(":login", pseudo);
    query.bindValue(":password", hashedPWD);
	query.exec();
    
    // If there is no match or if the user is already logged in, we cannot login
    if (!query.first() || query.record().value("isConnected").toBool()) {return false;}
    
    
    // Login successful, we have to set this user as logged in in the database
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
    
    // First, we get every room where this user is in
    query.prepare("SELECT idRoom FROM roomMembership INNER JOIN privilege ON roommembership.idPrivilege = privilege.idPrivilege WHERE idUser = :id AND (name = 'admin' OR name = 'user')");
	query.bindValue(":id", id);
	query.exec();
    
    while(query.next())
    {
        rooms.insert(query.record().value(0).toUInt());
    }
    
    // Then we get all the informations about him
    query.prepare("SELECT idUser, login, firstName, lastName, lastConnection, profilePicture, isConnected FROM user WHERE idUser = :id");
	query.bindValue(":id", id);
	query.exec();

    query.first();
    
    // We can finally build a ModelUser
    ModelUser user(query.record().value("idUser").toUInt(), query.record().value("login").toString(), query.record().value("firstName").toString(), query.record().value("lastName").toString(), query.record().value("isConnected").toBool(), query.record().value("lastConnection").toDateTime(), QImage(PROFILE_PICTURE_FOLDER + query.record().value("profilePicture").toString()), rooms);
    
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
    // Get the members and the adins
    QSet<quint32> admins;
    QSet<quint32> users;
    
    QSqlQuery query(_db);
	query.prepare("SELECT roomMembership.idUser, name FROM roomMembership INNER JOIN privilege ON roomMembership.idPrivilege = privilege.idPrivilege INNER JOIN user ON roomMembership.idUser = user.idUser WHERE idRoom = :idRoom AND (name = 'admin' OR name = 'user')");
	query.bindValue(":idRoom", id);
	query.exec();

    quint32 idUser;
    
    while(query.next())
    {
        idUser = query.record().value("idUser").toUInt();
        if (query.record().value("name").toString() == "admin") {admins.insert(idUser);}
        users.insert(idUser);
    }
    
    // Get the messages
    QMap<quint32, ModelMessage> messages;
	
	query.prepare("SELECT idMessage, idRoom, idUser, date, lastUpdated, contents FROM message WHERE idRoom = :idRoom ORDER BY date desc LIMIT (SELECT limitOfStoredMessages FROM room WHERE idRoom = :idRoom)");
	query.bindValue(":idRoom", id);
	query.exec();
    
    while(query.next())
    {
        ModelMessage message(query.record().value("idMessage").toUInt(), query.record().value("idRoom").toUInt(), query.record().value("idUser").toUInt(), query.record().value("date").toDateTime(), query.record().value("lastUpdated").toDateTime(), query.record().value("contents").toByteArray());
        messages.insert(message.getIdMessage(), message);
    }
    
    // Get the room informations
    query.prepare("SELECT idRoom, name, private, visible, picture, limitOfStoredMessages FROM room WHERE idRoom = :idRoom");
	query.bindValue(":idRoom", + id);
    query.exec();
    query.first();
    
    // We can finally build the room
    
    ModelRoom room(query.record().value("idRoom").toUInt(), query.record().value("name").toString(), query.record().value("limitOfStoredMessages").toUInt(), query.record().value("private").toBool(), query.record().value("visible").toBool(), QImage(PROFILE_PICTURE_FOLDER + query.record().value("picture").toString()), admins, users, messages);

    return room;
}

quint32 ControllerDB::createRoom(ModelRoom& room)
{
    // First, if the image is empty, we load the default image
    if (room.getPicture().isNull()) {room.setPicture(QImage(PROFILE_PICTURE_FOLDER + DEFAULT_ROOM_PICTURE));}
    
    // Create the room
    QSqlQuery query(_db);
	query.prepare("INSERT INTO room (name, limitOfStoredMessages, private, visible, picture) VALUES (:nameRoom, :limitRoom, :isPrivate, :isVisible, :picture)");
    query.bindValue(":nameRoom", room.getName());
	query.bindValue(":limitRoom", room.getLimit());
	query.bindValue(":isPrivate", QString::number(room.isPrivate()? 1 : 0));
	query.bindValue(":isVisible", QString::number(room.isVisible()? 1 : 0));
	query.bindValue(":picture", saveImage(room.getPicture()));
	query.exec();
    
    quint32 idRoom = query.lastInsertId().toUInt();
    
    
    // Creating the memberships for every admin and user
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
    // First, if the image is empty, we load the default image
    if (user.getImage().isNull()) {user.setImage(QImage(PROFILE_PICTURE_FOLDER + DEFAULT_PROFILE_PICTURE));}
    quint64 msecs = saveImage(user.getImage());
    
    // We cannot create the account if the user already exists
    quint32 id;
    if (userExists(user.getUserName(), id)) {return  false;}
    
    
    // We can create the user
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
    
    return true;
}

quint64 ControllerDB::saveImage(const QImage& image)
{
    // We try to create a file whose name is the current timestamp
    QFile picture;
    quint64 msecs = QDateTime::currentDateTime().toMSecsSinceEpoch();
    
    // But we can increment the timestamp manually if this name is already taken
    do
    {
        picture.setFileName(PROFILE_PICTURE_FOLDER + QString::number(msecs));
    } while (picture.exists() && msecs++);
    
    // We can save the image
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

void ControllerDB::modifyUser(const ModelUser& user, const QByteArray& password, const QByteArray& privateKey)
{
    QSqlQuery query(_db);
	query.prepare("UPDATE user (login, firstName, lastName, password, privateKey, isConnected) SET VALUES (:userName, :userFirstName, :userLastName, :password, :privateKey, 1) WHERE idUser = :idUser");
	query.bindValue(":userName", user.getUserName());
	query.bindValue(":userFirstName", user.getFirstName());
	query.bindValue(":userLastName", user.getLastName());
	query.bindValue(":password", password);
	query.bindValue(":privateKey", privateKey);
	query.bindValue(":idUser", user.getIdUser());
	query.exec();
	
	query.prepare("SELECT profilePicture FROM user WHERE idUser = :idUser");	
	query.bindValue(":idUser", user.getIdUser());
	query.exec();

    query.first();
    
    // We have to update the file in the filesystem
    QFile profilePicture(query.record().value("profilePicture").toString());
    profilePicture.open(QIODevice::WriteOnly);
    user.getImage().save(&profilePicture, PROFILE_PICTURE_FORMAT);
    profilePicture.close();
}

void ControllerDB::modifyRoom(const ModelRoom& room)
{
    QSqlQuery query(_db);
    query.prepare("UPDATE room SET name = :name, limitOfStoredMessages = :limit WHERE idRoom = :idRoom");
    query.bindValue(":name", room.getName());
    query.bindValue(":limit", room.getLimit());
    query.bindValue(":idRoom", room.getIdRoom());
    query.exec();
    
    query.exec("SELECT picture FROM room WHERE idRoom = :idRoom");
    query.bindValue(":idRoom", room.getIdRoom());
    query.exec();
    query.first();
    
    // We have to update the file in the filesystem
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

void ControllerDB::leaveRoom(const quint32 idUser, const quint32 idRoom)
{
    QSqlQuery query(_db);
    
    query.prepare("DELETE FROM roomMembership WHERE idUser = :idUser AND idRoom = :idRoom");
    query.bindValue(":idUser", idUser);
    query.bindValue(":idRoom", idRoom);
    query.exec();
    
    // After the deletion, if the room is empty, we have to delete it
    query.prepare("SELECT COUNT(idUser) AS nbMembers FROM roomMembership WHERE idRoom = :idRoom");
    query.bindValue(":idRoom", idRoom);
    query.exec();
    
    query.first();
    if (query.record().value("nbMembers").toUInt() == 0) {deleteRoom(idRoom);}
}

QByteArray ControllerDB::getSalt(const QString& pseudo)
{
    quint32 id;
    if (!userExists(pseudo, id)) {return QByteArray();}
    
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

bool ControllerDB::requestAccess(const quint32 idUser, const quint32 idRoom)
{
    QSqlQuery query(_db);
    
    // We first check that the request doesn't exist
    query.prepare("SELECT count(idUser) AS nbRequest FROM roomMembership  WHERE idUser = :idUser AND idRoom = :idRoom AND idPrivilege = (SELECT idPrivilege FROM privilege WHERE name = 'request')");
    query.bindValue(":idUser", idUser);
    query.bindValue(":idRoom", idRoom);
    query.exec();
    
    query.first();
    
    if (query.record().value("nbRequest").toUInt() > 0) {return false;}
    
    // We can create the request
    query.prepare("INSERT INTO roomMembership (idUser, idRoom, idPrivilege) VALUES (:idUser, :idRoom, (SELECT idPrivilege FROM privilege WHERE name = 'request'))");
    query.bindValue(":idUser", idUser);
    query.bindValue(":idRoom", idRoom);
    query.exec();
    
    return true;
}

void ControllerDB::setKey(const quint32 idUser, const quint32 idRoom, const QByteArray& aesKey)
{
    QSqlQuery query(_db);
    query.prepare("UPDATE roomMembership SET roomKey = :key WHERE idUser = :idUser AND idRoom = :idRoom");
    query.bindValue(":key", aesKey);
    query.bindValue(":idUser", idUser);
    query.bindValue(":idRoom", idRoom);
    query.exec();
}

QList<QPair<quint32, QString>> ControllerDB::listPublicRooms()
{
    QList<QPair<quint32, QString>> rooms;
    
    QSqlQuery query(_db);
    query.exec("SELECT idRoom, name FROM room WHERE private = 0");
    
    while (query.next())
    {
        rooms.append(QPair<quint32, QString>(query.record().value("idRoom").toUInt(), query.record().value("name").toString()));
    }
    
    return rooms;
}

QList<QPair<quint32, QString>> ControllerDB::listPrivateVisibleRooms()
{
    QList<QPair<quint32, QString>> rooms;
    
    QSqlQuery query(_db);
    query.exec("SELECT idRoom, name FROM room WHERE private = 1 AND visible = 1");
    
    while (query.next())
    {
        rooms.append(QPair<quint32, QString>(query.record().value("idRoom").toUInt(), query.record().value("name").toString()));
    }
    
    return rooms;
}

void ControllerDB::getCryptoData(const quint32 id, QByteArray& keySalt, QByteArray& publicKey, QByteArray& privateKey)
{
    QSqlQuery query(_db);
    query.prepare("SELECT saltKey, publicKey, privateKey FROM user WHERE idUser = :idUser");
    query.bindValue(":idUser", id);
    
    query.exec();
    query.first();
    
    keySalt = query.record().value("saltKey").toByteArray();
    publicKey = query.record().value("publicKey").toByteArray();
    privateKey = query.record().value("privateKey").toByteArray();
}

void ControllerDB::modifyMembership(const quint32 idRoom, const QSet<quint32>& newUsers, const QSet<quint32>& removedUsers, const QSet<quint32>& newAdmins, const QSet<quint32>& removedAdmins, const QMap<quint32, QByteArray>& usersAndKeys)
{
    QSqlQuery query(_db);
    QVariantList ids;
    
    // We first process the removed admins
    if (!removedAdmins.empty())
    {
        // Here, it is not a problem to directly include the room ID in the text, because it is an integer and isn't subject to SQL injection.
        query.prepare("UPDATE roomMembership SET idPrivilege = (SELECT idPrivilege FROM privilege WHERE name = 'user') WHERE idRoom = " + QString::number(idRoom) + " AND idUser = ?");
        
        // Now we can use a batch to process the data
        for (quint32 id : removedAdmins) {ids << id;}
        query.addBindValue(ids);
        
        query.execBatch();
    }
    
    
    // Then the removed users
    if (!removedUsers.empty() || !newUsers.empty())
    {
        query.prepare("DELETE FROM roomMembership WHERE idRoom = " + QString::number(idRoom) + " AND idUser = ?");
        
        ids.clear();
        for (quint32 id : removedUsers + newUsers) {ids << id;}
        query.addBindValue(ids);
        
        query.execBatch();
    }
    
    
    // We add the new users
    if (!newUsers.empty())
    {
        query.prepare("INSERT INTO roomMembership (idRoom, idPrivilege, idUser, roomKey) VALUES (" + QString::number(idRoom) + ", (SELECT idPrivilege FROM privilege WHERE name = 'user'), ?, ?)");
        
        ids.clear();
        QVariantList keys;
        for (quint32 id : newUsers) {ids << id; keys << usersAndKeys[id];}
        query.addBindValue(ids);
        query.addBindValue(keys);
        
        query.execBatch();
    }
    
    
    // Finally we grant the new admins
    if (!newAdmins.empty())
    {
        query.prepare("UPDATE roomMembership SET idPrivilege = (SELECT idPrivilege FROM privilege WHERE name = 'admin') WHERE idRoom = " + QString::number(idRoom) + " AND idUser = ?");
        
        ids.clear();
        for (quint32 id : newAdmins) {ids << id;}
        query.addBindValue(ids);
        
        query.execBatch();
    }
}

void ControllerDB::acceptOrDeny(const quint32 idRoom, const quint32 idUser, const QByteArray& key, const bool accepted)
{
    QSqlQuery query(_db);
    
    if (accepted)
    {
        // We set the privilege to user and store the key
        query.prepare("UPDATE roomMembership SET idPrivilege = (SELECT idPrivilege FROM privilege WHERE name = 'user'), roomKey = :key WHERE idUser = :idUser AND idRoom = :idRoom");
        query.bindValue(":key", key);
    }
    else
    {
        // The request is removed
        query.prepare("DELETE FROM roomMembership WHERE idUser = :idUser AND idRoom = :idRoom");
    }
    
    query.bindValue(":idUser", idUser);
    query.bindValue(":idRoom", idRoom);
    query.exec();
}

QByteArray ControllerDB::getAesKey(const quint32 idUser, const quint32 idRoom)
{
    QSqlQuery query(_db);
    query.prepare("SELECT roomKey FROM roomMembership WHERE idUser = :idUser AND idRoom = :idRoom");
    query.bindValue(":idUser", idUser);
    query.bindValue(":idRoom", idRoom);
    query.exec();
    
    query.first();
    
    return query.record().value("roomKey").toByteArray();
}

QList<QPair<quint32, QByteArray>> ControllerDB::getRequests(const quint32 idRoom)
{
    QSqlQuery query(_db);
    query.prepare("SELECT roomMembership.idUser, publicKey FROM roomMembership INNER JOIN user ON roomMembership.idUser = user.idUser WHERE idPrivilege = (SELECT idPrivilege FROM privilege WHERE name = 'request') AND idRoom = :idRoom");
    query.bindValue(":idRoom", idRoom);
    query.exec();
    
    QList<QPair<quint32, QByteArray>> requests;
    
    while (query.next())
    {
        requests.append(QPair<quint32, QByteArray>(query.record().value("idUser").toUInt(), query.record().value("publicKey").toByteArray()));
    }
    
    return requests;
}

void ControllerDB::cleanDatabase()
{
    qDebug() << "Database cleaning...";
    
    QSqlQuery query(_db);
    QSqlQuery queryDelete(_db);
    query.exec("SELECT idRoom, limitOfStoredMessages FROM room");
    
    while (query.next())
    {
        // We delete every message too old to be store, according to the limit of the room
        queryDelete.prepare("DELETE FROM message WHERE idRoom = :idRoom AND idMessage < (SELECT MIN(idMessage) FROM (SELECT idMessage FROM message WHERE idRoom = :idRoom ORDER BY date DESC LIMIT :limit))");
        
        queryDelete.bindValue(":idRoom", query.record().value("idRoom").toUInt());
        queryDelete.bindValue(":limit", query.record().value("limitOfStoredMessages").toUInt());
        queryDelete.exec();
    }
    
    query.exec("INSERT INTO cleaning (dateCleaning) VALUES (datetime('NOW'))");
    
    // We restart the timer for the next cleaning
    _timer.setInterval(CLEANING_INTERVAL * 1000);
    _timer.start();
}

ControllerDB::~ControllerDB() {}
