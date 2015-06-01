/*
     * This class is able to serialize and deserialize every packet defined in
     * our protocol. When the server or the client has something to send, it
     * calls this class which will serialize data whose can be sent afterwards
     * in the socket directly.
     * When the data arrives in the remote socket, the interpretor is called to
     * deserialize its content and send it to the controllerInput who knows
     * which controller can process it.
*/

#ifndef INTERPRETOR_H
#define INTERPRETOR_H

#include "packet.h"
#include "../../ModeleChator/modelChator.h"
#include <QByteArray>
#include <QByteArray>
#include <QList>
#include <QPair>
#include "../controllerInput/controllerInput.h"
#include "../ServerConnector/connector.h"

class Interpretor : public QObject
{
    Q_OBJECT
    
    public:
    
    // Constructor, takes a reference to the controllerInput to which the data will have to be transmitted 
    Interpretor(ControllerInput& dispatcher);

    //----------------------------------------------------------------------------------
    // Goal      : Serialize a message and the flag indicating if the message was edited
    //             or created.
    // Params    : message - message to send
    //             edited - flag
    //----------------------------------------------------------------------------------
    QByteArray sendMessage(const ModelMessage& message, const bool edited = false);

    //----------------------------------------------------------------------------------
    // Goal      : Serialize a message's id and its room's id, in the purpose of delete
    //             it.
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
    QByteArray editAccount(const ModelUser& user, const QByteArray& password = QByteArray(), const QByteArray& privateKey = QByteArray());

    //----------------------------------------------------------------------------------
    // Goal      : Serialize encrypted informations such as keySalt and
    //             rsaKeys. Once the user has received it, he can use his password to
    //             decipher it and store it in the local app. It will allow him to
    //             decipher encrypted messages addressed to him.
    // Params    : user - ModelUser of the current user
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
    //----------------------------------------------------------------------------------
    QByteArray room(const ModelRoom& room, const QMap<quint32, QByteArray>& usersAndKeys, bool edited);
    

    //----------------------------------------------------------------------------------
    // Goal      : Serialize a user name (packet is used to ask the server if a username
    //             is used and obtain the user's id if he exists).
    //             userId and exists are not used by the client, the server use them to answer the clien request.
    //             The message type will be USER_ID. The parameters are written in the same order they appear in the prototype.
    //----------------------------------------------------------------------------------
    QByteArray userId(const QString& userName, bool exists, quint32 userId);

    //----------------------------------------------------------------------------------
    // Goal      : Serialize a room's id to delete it.
    //----------------------------------------------------------------------------------
    QByteArray deleteRoom(const quint32 roomId);
    
    //----------------------------------------------------------------------------------
    // Goal      : Serialize a request that has been, or will have to be managed by an admin.
    //----------------------------------------------------------------------------------
    QByteArray request(const quint32 roomId, const ModelUser& user, const QByteArray& publicKey, const bool accepted);

    
    public slots:
    
    //----------------------------------------------------------------------------------
    // Goal      : Deserializes the data and transmits the data to the controllerInput
    //----------------------------------------------------------------------------------
    void processData(const QByteArray& data);
    
    private:
    ControllerInput& _dispatcher;
};

#endif
