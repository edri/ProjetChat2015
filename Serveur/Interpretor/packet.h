/*
     * Created by Benoist Wolleb
     *
     * Contains ID of every packet's kind so the server and the user
     * know what kind of packet it will process because this ID is
     * the first element sent (and received).
     * Also overload streams operator in order to ease de serialization.
*/

#ifndef PACKET_H
#define PACKET_H

#include <QtGlobal>
#include <QString>
#include <QDataStream>

enum class MessageType : quint32 {NEW_ACCOUNT, EDIT_ACCOUNT, INFO_USER, MESSAGE, DELETE_MESSAGE,
                                  LOGIN, USER_JOIN, JOIN, LEAVE, DISCONNECT, CONNECTED, SERVER_ERROR,
                                  ROOM, USER_ID, DELETE_ROOM, SALT, PUBLIC_KEY, LIST_ROOMS, REQUEST};
enum class ErrorType : quint32 {AUTH_ERROR, USER_CREATION, ROOM_CREATION};

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
