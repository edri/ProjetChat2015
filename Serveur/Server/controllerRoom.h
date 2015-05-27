/*
     * Created by Benoist Wolleb
     *
     * Processed requests sent by users and related to rooms such
     * as leaving a room, creating a room.
*/

#ifndef CONTROLLER_ROOM_H
#define CONTROLLER_ROOM_H

#include <QtGlobal>
#include "controllerDB.h"
#include "../../ModeleChator/modelChator.h"
#include "../Interpretor/interpretor.h"
#include "controllerUser.h"

class ControllerUser;

class ControllerRoom
{
    friend class ControllerUser;
    
    public:
    ControllerRoom(ControllerDB& db);
    void processMessage(ModelMessage& message, const bool edited, ChatorClient* client);
    void userConnected(const ModelUser& user, ChatorClient* client);
    void leaveRoom(const quint32 idUser, const quint32 idRoom, ChatorClient* client);
    void joinRoom(const quint32 idRoom, ChatorClient* client);
    void createRoom(ModelRoom& room, QList<quint32> usersIds, QList<QPair<QByteArray, QByteArray>> cryptedKeys, ChatorClient* client);
    void modifyRoom(ModelRoom& room, QList<quint32> usersIds, QList<QPair<QByteArray, QByteArray>> cryptedKeys, ChatorClient* client);
    void deleteRoom(const quint32 roomId, ChatorClient* client);
    void deleteMessage(const quint32 roomId, const quint32 messageId, ChatorClient* client);
    
    Interpretor* _interpretor;
    ControllerUser* _user;
    
    private:
    ControllerDB& _db;
    QMap<quint32, ChatorRoom*> _onlineRooms;
};

#endif
