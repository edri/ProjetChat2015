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
    
    //----------------------------------------------------------------------------------
    // Note      : All functions below need to receive a parameter ChatorClient.
    //             The ChatorClient contains a socket and allow communication between
    //             client and server.
    //             See the ModelChator.h for further information.
    //----------------------------------------------------------------------------------
    public:
    ControllerRoom(ControllerDB& db);

    //----------------------------------------------------------------------------------
    // Goal      : Process a message by verifiying if the messages is genuine (sent by a
    //             connected user, in a room he belongs. This function can process both
    //             original and modified message. After asking the database to store the
    //             the message, it will be sent to other users.
    // Param     : message - Original or modified message
    //             edited - Inform if it is an original or modified message
    //----------------------------------------------------------------------------------
    void processMessage(ModelMessage& message, const bool edited, ChatorClient* client);

    //----------------------------------------------------------------------------------
    // Goal      : Bind the user to the room as connected and inform other users of the
    //             same rooms that a user is logged.
    // Param     : user - The user
    //----------------------------------------------------------------------------------
    void userConnected(const ModelUser& user, ChatorClient* client);

    //----------------------------------------------------------------------------------
    // Goal      : When a user is leaving a room, update de database and inform other
    //             users of the room
    // Param     : idUser - User's identifier
    //             idRoom - Room's identifier
    //----------------------------------------------------------------------------------
    void leaveRoom(const quint32 idUser, const quint32 idRoom, ChatorClient* client);

    //----------------------------------------------------------------------------------
    // Goal      : When a user is joining a room, update de database and inform other
    //             users of the room. If the room is private, sent a notification to
    //             admins informing that a user ask to join the room.
    // Param     : idRoom - Room's identifier
    //----------------------------------------------------------------------------------
    void joinRoom(const quint32 idRoom, ChatorClient* client);

    //----------------------------------------------------------------------------------
    // Goal      : Create a new room in the database and sent encrypted key to the
    //             member of the room.
    // Param     : room - ModelRoom containing every information about the room
    //             usersAndKeys - User's public keys used to encrypted packets.
    //----------------------------------------------------------------------------------
    void createRoom(ModelRoom& room, const QMap<quint32, QByteArray>& usersAndKeys, ChatorClient* client);

    //----------------------------------------------------------------------------------
    // Goal      : Modify a room in the database and inform other users of the
    //             modifications.
    // Param     : room - The ModelRoom corresponding to the modified room.
    //             usersAndKeys - User's public keys used to encrypted packets.
    //----------------------------------------------------------------------------------
    void modifyRoom(ModelRoom& room, const QMap<quint32, QByteArray>& usersAndKeys, ChatorClient* client);

    //----------------------------------------------------------------------------------
    // Goal      : Delete a room from the database and inform other users.
    // Param     : roomId - Room's identifier.
    //----------------------------------------------------------------------------------
    void deleteRoom(const quint32 roomId, ChatorClient* client);

    //----------------------------------------------------------------------------------
    // Goal      : Delete a message from the database and inform other users in the room
    //             from where the message was deleted.
    // Param     : roomId - Room's identifier
    //             messageId - Message's identifiers
    //----------------------------------------------------------------------------------
    void deleteMessage(const quint32 roomId, const quint32 messageId, ChatorClient* client);

    //----------------------------------------------------------------------------------
    // Goal      : Grant or refuse access to a private room for a user. Only admins can
    //             do this action. If accepted, the user is add to the room.
    // Param     : idRoom - Room's identifier
    //             idUser - User's identifier
    //             key - AES Key
    //             accepted - Admin decision to grant or not access.
    //----------------------------------------------------------------------------------
    void acceptOrDeny(const quint32 idRoom, const quint32 idUser, const QByteArray& key, const bool accepted);
    
    Interpretor* _interpretor;
    ControllerUser* _user;
    
    private:
    ControllerDB& _db;
    QMap<quint32, ChatorRoom*> _onlineRooms;
};

#endif
