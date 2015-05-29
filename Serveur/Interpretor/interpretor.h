/*
     * Created by Benoist Wolleb
     *
     * Serialize any request made by the user in ControlerOutput in order
     * to be sent to the server.
     * Also serialize the other way, from server to client.
*/

#ifndef INTERPRETOR_H
#define INTERPRETOR_H

#include "../../ModeleChator/modelChator.h"
#include "packet.h"
//#include <QObject>
#include <QByteArray>
#include <QByteArray>
#include <QList>
#include <QPair>
#include "../controllerInput/controllerInput.h"
#include "../ServerConnector/connector.h"

//class Interpretor : public QObject        // Not sure yet if the Interpretor has to be a subclass of QObject (will it have signals/slots?)
class Interpretor : public QObject
{
    Q_OBJECT
    
    public:
    Interpretor(ControllerInput& dispatcher);

    //----------------------------------------------------------------------------------
    // Goal      : Serialize a message and the flag indicating if the message was edited
    //             or created.
    // Last edited by Miguel Santamaria on 08.05.2015 07:21
    //----------------------------------------------------------------------------------
    QByteArray sendMessage(const ModelMessage& message, const bool edited = false);

    //----------------------------------------------------------------------------------
    // Goal      : Serialize a message's id and its room's id, in the purpose of delete
    //             it.
    // Last edited by Miguel Santamaria on 18.05.2015 16:47
    //----------------------------------------------------------------------------------
    QByteArray deleteMessage(const quint32 roomId, const quint32 messageId);

    //----------------------------------------------------------------------------------
    // Goal      : Serialize a username and the hash of the submitted password in order
    //             to log in the chat.
    //----------------------------------------------------------------------------------
    QByteArray login(const QString& pseudo, const Hash& hashedPwd);

    //----------------------------------------------------------------------------------
    // Goal      : Serialize a ModelUser containing the new user's information and
    //             several information used to guarantee security.
    //----------------------------------------------------------------------------------
    QByteArray createAccount(const ModelUser& user, const Hash& password, const Salt& passwordSalt, const Salt& keySalt, const RSAPair& asymKeys);

    //----------------------------------------------------------------------------------
    // Goal      : Serialize a ModelUser containing modified information about the
    //             current user.
    //----------------------------------------------------------------------------------
    QByteArray editAccount(const ModelUser& user, const QByteArray& keySalt = QByteArray(), const QByteArray& privateKey = QByteArray());

    //----------------------------------------------------------------------------------
    // Goal      : Serialize encrypted informations such as keySalt and
    //             rsaKeys. Once the user has received it, he can use his password to
    //             decipher it and store it in the local app. It will allow him to
    //             decipher encrypted messages addressed to him.
    // Param     : user - cuModelUser of the current user
    //             keySalt - salt used for key's encryption
    //             rsaKeys - RSA Keys
    //----------------------------------------------------------------------------------
    QByteArray sendInfoUser(const ModelUser& user, const QByteArray& keySalt = QByteArray(), const QByteArray& privateKey = QByteArray(), const QByteArray& publicKey = QByteArray());

    //----------------------------------------------------------------------------------
    // Goal      : Serialize various errors
    //----------------------------------------------------------------------------------
    QByteArray sendError(const ModelError& error);

    //----------------------------------------------------------------------------------
    // Goal      : Everytime a user joins a room, idUser and idRoom are serialize and
    //             sent to clients.
    //----------------------------------------------------------------------------------
    QByteArray userJoin(const quint32 idUser, const quint32 idRoom);

    //----------------------------------------------------------------------------------
    // Goal      : Allow to serialize a list of user membership in rooms
    //----------------------------------------------------------------------------------
    QByteArray join(const QMap<quint32, ModelRoom>& rooms, const QMap<quint32, ModelUser>& users);

    //----------------------------------------------------------------------------------
    // Goal      : Serialize a packet which inform the users that another user has
    //             logged on.
    //----------------------------------------------------------------------------------
    QByteArray connected(const ModelUser& user);

    //----------------------------------------------------------------------------------
    // Goal      : Serialize a packet which inform the users that another user has
    //             disconnected.
    //----------------------------------------------------------------------------------
    QByteArray disconnect(const quint32 idUser);

    //----------------------------------------------------------------------------------
    // Goal      : Serialize the salt and the username corresponding so the user can
    //             hash his password and send it back.
    //----------------------------------------------------------------------------------
    QByteArray salt(const QString& pseudo, const QByteArray& salt = QByteArray());

    //----------------------------------------------------------------------------------
    // Goal      : Serialize the public key
    //----------------------------------------------------------------------------------
    QByteArray publicKey(const QList<QPair<quint32, QByteArray>>& usersIdAndKey);

    //----------------------------------------------------------------------------------
    // Goal      : Serialize a user and a room he has just left.
    //----------------------------------------------------------------------------------
    QByteArray leave(const quint32 idUser, const quint32 idRoom);

    //----------------------------------------------------------------------------------
    // Goal      : Serialize the list of public room and the list of visible private
    //             room so these rooms are display in the chat interface.
    //----------------------------------------------------------------------------------
    QByteArray listRooms(const QList<QPair<quint32, QString>>& publicRooms = QList<QPair<quint32, QString>>(), const QList<QPair<quint32, QString>>& privateVisibleRooms = QList<QPair<quint32, QString>>());
    QByteArray joinRoom(const quint32 roomId);
    
    //----------------------------------------------------------------------------------
    // Goal      : Serialize a room and the flage indicating if the room was edited or
    //             created.
    //             The message type will be ROOM.
    //             The flag is written first, the room second.
    // Created Jan Purro 26.04.2015 14:55 modified by Jan Purro 26.05.2015 00:36
    //----------------------------------------------------------------------------------
    QByteArray room(const ModelRoom& room, QList<quint32>& usersIds, QList<QPair<QByteArray, QByteArray>>& cryptedKeys , bool edited);
    

    //----------------------------------------------------------------------------------
    // Goal      : Serialize a user name (packet is used to ask the server if a username
    //             is used and obtain the user's id if he exists).
    //             userId and exists are not used by the client, the server use them to answer the clien request.
    //             The message type will be USER_ID. The parameters are written in the same order they appear in the prototype.
    // Created Jan Purro 27.04.2015 13:35
    //----------------------------------------------------------------------------------
    QByteArray userId(const QString& userName, bool exists, quint32 userId);

    //----------------------------------------------------------------------------------
    // Goal      : Serialize a room's id.
    // Created by Miguel Santamaria on 13.05.2015 09:04
    //----------------------------------------------------------------------------------
    QByteArray deleteRoom(const quint32 roomId);


    QByteArray request(const quint32 roomId, const ModelUser& user, const QByteArray& publicKey, const bool accepted);

    //QByteArray leaveRoom(const quint32 roomId);
    
    public slots:
    void processData(const QByteArray& data);
    
    private:
    ControllerInput& _dispatcher;
};

#endif
