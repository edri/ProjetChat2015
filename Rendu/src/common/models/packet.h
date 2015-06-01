/*
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

// Types of packets
enum class MessageType : quint32 {NEW_ACCOUNT, EDIT_ACCOUNT, INFO_USER, MESSAGE,
                                  DELETE_MESSAGE, LOGIN, JOIN, LEAVE, DISCONNECT,
                                  CONNECTED, SERVER_ERROR, ROOM, USER_ID,
                                  DELETE_ROOM, SALT, PUBLIC_KEY, LIST_ROOMS,
                                  REQUEST, JOIN_ROOM};

// Types of errors
enum class ErrorType : quint32 {AUTH_ERROR, USER_CREATION, ROOM_CREATION};

#endif
