#ifndef CHATOR_CONSTANTS_H
#define CHATOR_CONSTANTS_H

#include <QtGlobal>
#include <QString>

// The filename of the database to use
const QString DATABASE_FILE_NAME = "db.sqlite";

// The database type to use
const QString DATABASE_TYPE = "QSQLITE";

// The script to use for the database initialization
const QString DATABASE_INIT_SCRIPT = "init.sql";

// The port to be listened
const quint16 PORT_TO_LISTEN = 1234;

// The folder where the profile pictures are stored
const QString PROFILE_PICTURE_FOLDER = "img/";

// The image format to use to save pictures
const char PROFILE_PICTURE_FORMAT[] = "JPG";

#endif