#ifndef PACKET_H
#define PACKET_H

#include <QtGlobal>
#include <QString>
#include <QDataStream>

enum class MessageType : quint32 {NEW_ACCOUNT, INFO_USER, MESSAGE, LOGIN, JOIN, LEAVE, DISCONNECT, SERVER_ERROR};
enum class ErrorType : quint32 {AUTH_ERROR};

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
