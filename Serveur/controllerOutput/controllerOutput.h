/*
     * Contains methods called everytime a user want to communicate with
     * the server.
*/

#ifndef CONTROLLER_OUTPUT_H
#define CONTROLLER_OUTPUT_H

#include <QtCore/QObject>
#include "../Interpretor/interpretor.h"

class ControllerOutput
{
    public:
    
    // Constructor
    ControllerOutput(Connector& connector, Interpretor& interpretor);
    
    /* Called when an user want to send a new message or edit one of his.
     * - messageId : the message's id we want to delete.
     * - edited : indicates if it concerns a new message or a modification.
     */
    void sendMessage(const ModelMessage& message, const bool edited);

    /* Called when an user want to delete its message.
     * - messageId : the message's id we want to delete.
     */
    void deleteMessage(const quint32 roomId, const quint32 messageId);
    
    /* Called when an user want to login
     * - pseudo : the username.
     * - hashedPwd : the hashed password.
     */
    void login(const QString& pseudo, const Hash& hashedPwd);
    
    /* Called when an user want to create a new account in the server
     * - user : the complete profile of the user to create.
     * - password : the desired hashed password.
     * - passwordSalt : the salt that hashes the password.
     * - keySalt : the salt that hashes the password to encrypt the RSA private key.
     * - asymKeys : RSA keyring.
     */
    void createAccount(const ModelUser& user, const Hash& password, const Salt& passwordSalt, const Salt& keySalt, const RSAPair& asymKeys);
    void editAccount(const ModelUser& user);
    void sendInfoUser(const ModelUser& user);
    
    /* Called when an user want to leave a room.
     * - roomId : the room's id the user want to leave.
     */
    void leave(const quint32 idUser, const quint32 idRoom);
    
    /* Called when the user wants to log out (currently not used).
     *  - userId : the id of the user.
     */
    void disconnect(const quint32 idUser);
    
    /* Called when the user wants to retrieve the public keys of other users.
     *  - usersIdAndKey : the ids of the users.
     */
    void publicKey(const QList<QPair<quint32, QByteArray>>& usersIdAndKey);
    
    /* Called when creating or editing a room.
     * - room : the created or edited room.
     * - edited : false if the room is created, true if the room is an already existing modfied room.
     */
    void room(const ModelRoom& room, const QMap<quint32, QByteArray>& usersAndKeys, bool edited = false);
    
    /* Called when asking the server for the ID corresponding to a user name, provided the user exists.
     *  - userName : the inquired user name.
     *  - exists : only used by the server when responding 
     *  - userId : only used by the server when responding 
     */
    void userId(const QString& userName, bool exists = false, quint32 userId = 0);

    /* Called when a room administrator want to delete it.
     * - roomId : the room's id we want to delete.
     */
    void deleteRoom(const quint32 roomId);


    /* Called when a user need a salt in order to hash his password
     * when he is trying to connect to the server
     * - pseudo : userName
     */
    void askForSalt(const QString& pseudo);
    
    /* Called when the clien needs to know receive a list of all the public
     * rooms as well as the visible private rooms
     */
    void askForRooms();
    
    void joinRoom(const quint32 roomId);

    /* Called when a private room's administrator accept or reject an
     * user's new membership request.
     * - roomId : the private room's id
     * - user : the object representing the membership-requester user
     * - publicKey :
     * - status : indicate whether the administrator accepted the user
     *            (true), or not (false).
     */
    void changeRequestStatus(const quint32 roomId, const ModelUser& user,
                             const QByteArray& publicKey, const bool accepted);


    private:
    Connector& _connector;
    Interpretor& _interpretor;
};

#endif
