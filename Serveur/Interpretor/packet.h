#ifndef PACKET_H
#define PACKET_H

#include <QtGlobal>
#include <QString>
#include <QDataStream>

enum class MessageType : quint32 {NEW_ACCOUNT, INFO_USER, MESSAGE, LOGIN, USER_JOIN, JOIN, LEAVE, DISCONNECT, CONNECTED, SERVER_ERROR, ROOM, USER_ID};
enum class ErrorType : quint32 {AUTH_ERROR, ROOM_CREATION};

class ModelError
{
    friend QDataStream& operator<< (QDataStream& ds, const ModelError& r);
    friend QDataStream& operator>> (QDataStream& ds, ModelError& r);
    
    public:
    ModelError();
    ModelError(ErrorType errorType, QString errorString);
    
    private:
    ErrorType _errorType;
    QString _errorString;
};

QDataStream& operator<< (QDataStream& ds, const ModelError& r);
QDataStream& operator>> (QDataStream& ds, ModelError& r);

#endif
