#ifndef PACKET_H
#define PACKET_H

#include <QtGlobal>
#include <QString>
#include <QDataStream>

enum class MessageType : quint32 {NEW_ACCOUNT, INFO_USER, MESSAGE, LOGIN, JOIN, LEAVE, DISCONNECT, ERROR};
enum class ErrorType : quint32 {AUTH_ERROR};

class Error
{
    friend QDataStream& operator<< (QDataStream& ds, const Error& r);
    friend QDataStream& operator>> (QDataStream& ds, Error& r);
    
    public:
    Error();
    Error(ErrorType errorType, QString errorString);
    
    private:
    ErrorType _errorType;
    QString _errorString;
};

QDataStream& operator<< (QDataStream& ds, const Error& r);
QDataStream& operator>> (QDataStream& ds, Error& r);

#endif
