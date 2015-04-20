#include "controllerDB.h"
#include <QtCore/QDebug>
#include <QFile>
#include <QSqlError>
#include <QSqlQuery>

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
