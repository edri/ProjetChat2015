/*
     * Created by Benoist Wolleb
     *
     * Contains methods used to interact with the SQLite database.
*/

#ifndef CONTROLLER_DB_H
#define CONTROLLER_DB_H

#include <QSqlDatabase>
#include <QString>
#include "../../ModeleChator/modelChator.h"
#include "../../ModeleChator/chatorConstants.h"
#include "serverModel.h"

class ControllerDB : public QObject
{
    Q_OBJECT
    
    public:
    //----------------------------------------------------------------------------------
    // Goal      : Constructor of the controllerDB
    // Param     : dbName - Name of the database that is used.
    //             See ModelChator\chatorConstants.h for further details
    //----------------------------------------------------------------------------------
    ControllerDB(const QString& dbName = DATABASE_FILE_NAME);
    ~ControllerDB();

    //----------------------------------------------------------------------------------
    // Goal      : Connect to the database, if the database does not exist, create a
    //             fresh one.
    // Param     : /
    //----------------------------------------------------------------------------------
    bool connect();

    //----------------------------------------------------------------------------------
    // Goal      : Initialize the database by executing the instructions int the SQLi
    //             script.
    // Param     : /
    //----------------------------------------------------------------------------------
    bool init();

    //----------------------------------------------------------------------------------
    // Goal      : Look in the databse if there is a match for the pseudo/hashedPWD and
    //             udpate user status to "connected".
    //             Return true if there is match
    //             Return false if there no match (wrong pseudo or hashed password).
    // Param     : pseudo - User that want to login
    //             hashedPWD - Hashed of the password for comparing in the DB
    //             id - User's identifier
    //----------------------------------------------------------------------------------
    bool login(const QString& pseudo, const QByteArray& hashedPWD, quint32& id);

    //----------------------------------------------------------------------------------
    // Goal      : Set user status to "disconnected"
    // Param     : userId - User's identifier
    //----------------------------------------------------------------------------------
    void logout(const quint32 userId);

    //----------------------------------------------------------------------------------
    // Goal      : Create a new account by the information provided by the user.
    //             Return true if the creation has succeeded.
    // Param     : user - User's personal information (username,full name, picture)
    //             password - Hashed password
    //             passwordSalt - Salt used in order to hash the password
    //             keySalt - Salt used in order to hash the private Key
    //             privateKey - Encrypted privateKey
    //             publicKey - Public key
    //----------------------------------------------------------------------------------
    bool createAccount(ModelUser& user, const QByteArray& password, const QByteArray& passwordSalt, const QByteArray& keySalt, const QByteArray& privateKey, const QByteArray& publicKey);

    //----------------------------------------------------------------------------------
    // Goal      : Return true if the user exist in the database
    // Param     : pseudo - username
    //             id - User's identifier
    //----------------------------------------------------------------------------------
    bool userExists(const QString& pseudo, quint32& id);

    //----------------------------------------------------------------------------------
    // Goal      : Get user's information from his ID.
    //             Return a ModelUser.
    // Param     : id - User's identifier
    //----------------------------------------------------------------------------------
    ModelUser info(const quint32 id);

    //----------------------------------------------------------------------------------
    // Goal      : Store a message in the database.
    // Param     : message - The message sent by a user
    //----------------------------------------------------------------------------------
    quint32 storeMessage(const ModelMessage& message);

    //----------------------------------------------------------------------------------
    // Goal      : Edit an existing message.
    // Param     : message - The modified message
    //---------------------------------------------------------------------------------
    void editMessage(const ModelMessage& message);

    //----------------------------------------------------------------------------------
    // Goal      : Delete a message from the database.
    // Param     : id - Message's identifier
    //----------------------------------------------------------------------------------
    void deleteMessage(const quint32 id);

    //----------------------------------------------------------------------------------
    // Goal      : Get room's information from its id.
    // Param     : id - Room's identifier
    //----------------------------------------------------------------------------------
    ModelRoom infoRoom(const quint32 id);

    //----------------------------------------------------------------------------------
    // Goal      : Create a new room
    // Param     : room - ModelRoom which store every information about the room. (name,
    //             picture, type of room, room's member, room's admin, picture, ...)
    //----------------------------------------------------------------------------------
    quint32 createRoom(ModelRoom& room);

    //----------------------------------------------------------------------------------
    // Goal      : Get message's information from its id.
    // Param     : id - Message's identifer
    //----------------------------------------------------------------------------------
    ModelMessage infoMessage(const quint32 id);

    //----------------------------------------------------------------------------------
    // Goal      : Modify an existing room
    // Param     : room - Update information of the room.
    //----------------------------------------------------------------------------------
    void modifyRoom(const ModelRoom& room);

    //----------------------------------------------------------------------------------
    // Goal      : Update current memberships related to the room. For example when a
    //             user join a room or when he is being promoted.
    //             The database has to be updated.
    // Param     :
    //----------------------------------------------------------------------------------
    void modifyMembership(const quint32 idRoom, const QSet<quint32>& newUsers, const QSet<quint32>& removedUsers = QSet<quint32>(), const QSet<quint32>& newAdmins = QSet<quint32>(), const QSet<quint32>& removedAdmins = QSet<quint32>(), const QMap<quint32, QByteArray>& usersAndKeys = QMap<quint32, QByteArray>());

    //----------------------------------------------------------------------------------
    // Goal      : Modify a currentUser.
    // Param     : user - ModelUser containing updated information.
    //             password - Hashed password
    //             privateKey -
    //----------------------------------------------------------------------------------
    void modifyUser(const ModelUser& user, const QByteArray& password, const QByteArray& privateKey);

    //----------------------------------------------------------------------------------
    // Goal      : Delete a room in the database and every dependencies such as room's
    //             membership and messages.
    // Param     : roomId - Room's identifer
    //----------------------------------------------------------------------------------
    void deleteRoom(const quint32 roomId);

    //----------------------------------------------------------------------------------
    // Goal      : Store an image in the server's filesystem, not in the database.
    // Param     : image - Image that has to be stored
    //----------------------------------------------------------------------------------
    quint64 saveImage(const QImage& image);

    //----------------------------------------------------------------------------------
    // Goal      : Remove a user from a room by updating the membership's table.
    // Param     : idUser - User's identifier.
    //             idRoom - Room's identifer.
    //----------------------------------------------------------------------------------
    void leaveRoom(const quint32 idUser, const quint32 idRoom);

    //----------------------------------------------------------------------------------
    // Goal      : Return a user's salt from the database.
    // Param     : pseudo - pseudo of the user who want to get his salt in order to hash
    //                      his password
    //----------------------------------------------------------------------------------
    QByteArray getSalt(const QString& pseudo);

    //----------------------------------------------------------------------------------
    // Goal      : Return a user's public key from the database.
    // Param     : idUser - user's identifier
    //----------------------------------------------------------------------------------
    QByteArray getPublicKey(const quint32 idUser);

    //----------------------------------------------------------------------------------
    // Goal      : Add a new request to join a private visible room.
    //             Return false if the user has already asked for the access.
    // Param     : idUser - Identify the user who want to join the room
    //             idRoom - Private visible room's identifer
    //----------------------------------------------------------------------------------
    bool requestAccess(const quint32 idUser, const quint32 idRoom);

    //----------------------------------------------------------------------------------
    // Goal      : Store the key corresponding to a room's membership.
    // Param     : idUser - User's identifier.
    //             idRoom - Room's identifer
    //             aesKey - Encrypted AES key
    //----------------------------------------------------------------------------------
    void setKey(const quint32 idUser, const quint32 idRoom, const QByteArray& aesKey);

    //----------------------------------------------------------------------------------
    // Goal      : Return a list of every public rooms.
    //             This list consists of pair roomId and room's name.
    // Param     : /
    //----------------------------------------------------------------------------------
    QList<QPair<quint32, QString>> listPublicRooms();

    //----------------------------------------------------------------------------------
    // Goal      : Return a list of every private visible rooms.
    //             This list consists of pair roomId and room's name.
    // Param     : /
    //----------------------------------------------------------------------------------
    QList<QPair<quint32, QString>> listPrivateVisibleRooms();

    //----------------------------------------------------------------------------------
    // Goal      : Retrieve any information requried for encryption and prepare them to
    //             be send by transforming them into byte array.
    // Param     : id - user's identifant.
    //             keySalt, publicKey, privateKey - These parameters are used as
    //             input/output parameters. Information are stored in the form of byte
    //             array.
    //----------------------------------------------------------------------------------
    void getCryptoData(const quint32 id, QByteArray& keySalt, QByteArray& publicKey, QByteArray& privateKey);

    //----------------------------------------------------------------------------------
    // Goal      : Accept or deny a user's access to a private room. If the user is
    //             accepted, his privilege change 'request' to 'user' and his
    //             membership's key is updated.
    //             If the user is refused, his request is deleted from the database.
    // Param     : idRoom - room's identifer
    //             idUser - user's identifer
    //             key - AES Key
    //             accepted - indicates whether the user is accepted or not.
    //----------------------------------------------------------------------------------
    void acceptOrDeny(const quint32 idRoom, const quint32 idUser, const QByteArray& key, const bool accepted);

    //----------------------------------------------------------------------------------
    // Goal      : Retrieve an encrypted AES key from a user's membership to a room.
    // Param     : idUser - user's identifer
    //             idRoom - room's identifer
    //----------------------------------------------------------------------------------
    QByteArray getAesKey(const quint32 idUser, const quint32 idRoom);
    
    QList<QPair<quint32, QByteArray>> getRequests(const quint32 idRoom);
    
    public slots:
    
    void cleanDatabase();
    
    private :
    QSqlDatabase _db;
    QTimer _timer;
};

#endif
