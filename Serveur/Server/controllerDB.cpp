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
    
    return true;
}

ModelUser ControllerDB::info(const quint32 id)
{
    QSqlQuery query(_db);
    query.prepare("SELECT idUser, login, firstName, lastName, lastConnection, profilePicture FROM user WHERE idUser = :id");
    query.bindValue(":id", id);
    query.first();
    
    return ModelUser(query.record().value("idUser").toUInt(), query.record().value("login").toString(), query.record().value("firstName").toString(), query.record().value("lastName").toString(), true, query.record().value("lastConnection").toDateTime(), query.record().value("profilePicture").toString());
}
