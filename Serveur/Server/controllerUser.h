/*
     * Created by Benoist Wolleb
     *
     * Process requests sent by users and related to users such
     * as login or salt request in order to hash his password and
     * interact with the database's controller (controllerDB.h)
*/

#ifndef CONTROLLER_USER_H
#define CONTROLLER_USER_H

#include "controllerDB.h"
#include "serverModel.h"
#include "../Interpretor/interpretor.h"
#include "controllerRoom.h"

class ControllerRoom;

class ControllerUser
{
    friend class ControllerRoom;
    

    //----------------------------------------------------------------------------------
    // Note      : All functions below need to receive a parameter ChatorClient.
    //             The ChatorClient contains a socket and allow communication between
    //             client and server.
    //             See the ModelChator.h for further information.
    //----------------------------------------------------------------------------------
    public:
    ControllerUser(ControllerDB& db);

    //----------------------------------------------------------------------------------
    // Goal      : Ask the database if the user has provided correct information.
    //             If the user is able to login, the controller will sent everything the
    //             need as information : the lists of rooms that he is part of and every
    //             user's which are in the rooms.
    //             The controller will also inform other users that this user is now
    //             connected.
    //             If the user provide wrong password or username, an error is sent.
    // Param     : pseudo - user name
    //             hashedPWD - Password previously hashed by the user
    //----------------------------------------------------------------------------------
    void login(const QString& pseudo, const QByteArray& hashedPWD, ChatorClient* client);

    //----------------------------------------------------------------------------------
    // Goal      : Retrieve the userId from a userName and sent it back to the client
    //----------------------------------------------------------------------------------
    void userId(const QString& userName, ChatorClient* client);

    //----------------------------------------------------------------------------------
    // Goal      : Create a new account for a user. See createAccount from controllerDB
    //             for further information.
    //             If the creation fails, sent back an error.
    //             If it success, call the login function.
    // Param     : user - ModelUser containing any information related to the user
    //             password, passwordSalt, keySalt,  privateKey, publicKey - These
    //             parameters allow to have an application with a certain level of
    //             security and are detailed in the project documentation.
    //----------------------------------------------------------------------------------
    void createAccount(ModelUser& user, const QByteArray& password, const QByteArray& passwordSalt, const QByteArray& keySalt, const QByteArray& privateKey, const QByteArray& publicKey, ChatorClient* client);

    //----------------------------------------------------------------------------------
    // Goal      : Called when a user disconnect from the chat. It wil ask the database to
    //             update the user's status and inform other user that this user
    //             disconnected.
    //----------------------------------------------------------------------------------
    void disconnect(ChatorClient* client);

    //----------------------------------------------------------------------------------
    // Goal      : Inform the database that a user has modified his account and sent
    //             send a packet to every user's who are in the same room as him that
    //             his profile was updated.
    //----------------------------------------------------------------------------------
    void modifyUser(const ModelUser& user, const QByteArray& password, const QByteArray& privateKey, ChatorClient* client);

    //----------------------------------------------------------------------------------
    // Goal      : Retrieve salt from the database and sent it to the client
    // Param     : pseudo - User who want to have his salt in order to hash his
    //             password.
    //----------------------------------------------------------------------------------
    void getSalt(const QString& pseudo, ChatorClient* client);

    //----------------------------------------------------------------------------------
    // Goal      : Retrieve a list of public keys so users can encrypt messages and sent
    //             to other users.
    // Param     :
    //----------------------------------------------------------------------------------
    void getPublicKeys(QList<QPair<quint32, QByteArray>>& usersIdAndKey, ChatorClient* client);

    //----------------------------------------------------------------------------------
    // Goal      : Retrieve public room and visible private room.
    //----------------------------------------------------------------------------------
    void getAvalaibleRooms(ChatorClient* client);
    
    Interpretor* _interpretor;
    ControllerRoom* _room;
    
    private:
    ControllerDB& _db;
    QMap<quint32, ChatorClient*> _connectedUsers;
};

#endif
