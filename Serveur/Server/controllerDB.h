#ifndef CONTROLLER_DB_H
#define CONTROLLER_DB_H

#include <QSqlDatabase>
#include <QString>
#include "../../ModeleChator/modelChator.h"

class ControllerDB
{
    public:
    ControllerDB(const QString& dbName = "db.sqlite");
    bool connect();
    bool init();
    bool login(const QString& pseudo, const QString& hashedPWD, quint32& id);
    ModelUser info(const quint32 id);
    
    private :
    QSqlDatabase _db;
};

#endif
