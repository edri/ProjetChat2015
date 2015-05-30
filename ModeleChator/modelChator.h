/*
     * Created by Miguel Santamaria
     *
     * This model is used to store any information received from the
     * server such as room, users and messages. It Contains any informations
     * related to the chat.
*/

#ifndef MODELCHATOR
#define MODELCHATOR

#include <QtWidgets>
#include <QString>
#include "../Cryptor/cryptor.h"

#include "../Cryptor/cryptor.h"

using namespace std;

class ModelUser;
class ModelMessage;
class ModelRoom;
class ModelRequest;

class ModelChator
{
	private :
        // The room, users and membership requests are stocked here.
        // Messages are stocked in their respective rooms.
        QMap<quint32, ModelRoom> _rooms;
        QMap<quint32, ModelUser> _users;
        QMap<quint32, ModelRequest> _requests;

        // Indicate the number of private room's membership requests.
        quint32 _nbRequests;

        RSAPair _rsaKeyPair;
	
	public :
    
        ModelChator();

        //----------------------------------------------------------------------------------
        // Goal      : Get all the given user's rooms, and return them in a QMap.
        // Used in : - ControllerChat::loadRooms.
        // Created by Miguel Santamaria, on 17.04.2015 21:20
        // Last edited by Jan Purro, on 24.04.2015 16:23
        //----------------------------------------------------------------------------------
        QList<quint32> getUserRooms(const quint32 idUser) const;

        //----------------------------------------------------------------------------------
        // Goal      : Add an user to the users list.
        //             Used anytime we need to store an user in the application.
        // Created by Miguel Santamaria, on 17.04.2015 21:59
        // Last edited by Jan Purro, on 24.04.2015 15:57
        //----------------------------------------------------------------------------------
        void addUser(const quint32 idUser, const QString& userName, const QString& firstName, const QString& lastName, const bool isConnected, const QDateTime& lastConnection, const QImage& image, const QSet<quint32> roomsIds);
        void addUser(const ModelUser& user);
        
        //----------------------------------------------------------------------------------
        // Goal      : Add an admin to the specified room
        // Last edited by Jan Purro, on 24.04.2015 16:31
        //----------------------------------------------------------------------------------
        void addAdmin(const quint32 idRoom, const quint32 idUser);

        //----------------------------------------------------------------------------------
        // Goal      : Get the user identified by the given id.
        //             Used anytime we need to get an user in the application.
        // Created by Miguel Santamaria, on 21.04.2015 09:12
        // Last edited by Jan Purro, on 24.04.2015 16:07
        //----------------------------------------------------------------------------------
        ModelUser& getUser(const quint32 idUser);
        const ModelUser& getUser(const quint32 idUser) const;

        //----------------------------------------------------------------------------------
        // Goal      : Add room to the rooms list.
        //             Used anytime we need to store a room in the application.
        // Created by Miguel Santamaria, on 18.04.2015 00:18
        // Last edited by Miguel Santamaria, on 18.04.2015 00:18
        //----------------------------------------------------------------------------------
        void addRoom(const quint32 idRoom, const QString &name, const quint32 limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QImage &picture, const QSet<quint32>& admins, const QSet<quint32>& users, const QMap<quint32, ModelMessage>& messages, const AESKey& secretKey);
        void addRoom(const ModelRoom& room);

        //----------------------------------------------------------------------------------
        // Goal      : Modify a specific room.
        // Param     : Get the room identified by the given id.
        //             Used anytime we need to get a room in the application.
        // Created by Miguel Santamaria, on 20.04.2015 12:22
        // Last edited by Jan Purro, on 24.04.2015 16:11
        //----------------------------------------------------------------------------------
        ModelRoom& getRoom(const quint32 idRoom);
        const ModelRoom& getRoom(const quint32 idRoom) const;
        
        bool containsRoom(const quint32 idRoom) const;
        

        //----------------------------------------------------------------------------------
        // Goal      : Modify a specific room.
        // Param     : name, limitOfStoredMessage, isPrivate, isVisible, picture
        //             New values for the room.
        // Last edited by Jan Purro on 24.04.2015 16:40
        //----------------------------------------------------------------------------------
        void modifyRoom(const quint32 idRoom, const QString& name, const quint32 limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QImage& picture);

        //----------------------------------------------------------------------------------
        // Goal      : Delete a specific room.
        // Param     : idRoom - Room's identifier
        //----------------------------------------------------------------------------------
        void deleteRoom(const quint32 idRoom);

        //----------------------------------------------------------------------------------
        // Goal      : Unsuscribe the given user to the given room.
        // Param     : userId - User's identifier
        //             idRoom - Room's identifier
        // Created by Miguel Santamaria on 14.05.2015 19:55
        //----------------------------------------------------------------------------------
        void removeUser(const quint32 userId, const quint32 roomId);

        //----------------------------------------------------------------------------------
        // Goal      : Return a specific message.
        // Param     : idRoom - Room's identifier
        //             idMessage - Message's identifer
        // Last edited by Jan Purro on 24.04.2015 16:47
        //----------------------------------------------------------------------------------
        ModelMessage& getMessage(const quint32 idRoom, const quint32 idMessage);

        //----------------------------------------------------------------------------------
        // Goal      : Modify a message
        // Param     : idRoom - Room's identifier
        //             idMessage - Message's identifer
        //             content - New message's content
        //             lastEditionDate - Update the message date.
        //----------------------------------------------------------------------------------
        void modifyMessage(const quint32 idRoom, const quint32 idMessage, const QByteArray& content, const QDateTime lastEditionDate);

        //----------------------------------------------------------------------------------
        // Goal      : Delete a message.
        // Param     : idRoom - Room's identifier
        //             idMessage - Message's identifer
        //----------------------------------------------------------------------------------
        void deleteMessage(const quint32 idRoom, const quint32 idMessage);

        //----------------------------------------------------------------------------------
        // Goal      : Add a new user's membership request in the model.
        // Created by Miguel Santamario on 29.05.2015 21:35
        //----------------------------------------------------------------------------------
        void addMembershipRequest(const quint32 roomId, const ModelUser& user, const QByteArray& publicKey);

        //----------------------------------------------------------------------------------
        // Goal      : Delete the given membership request in the model.
        // Created by Miguel Santamario on 30.05.2015 15:01
        //----------------------------------------------------------------------------------
        void deleteRequest(const quint32 requestId);

        //----------------------------------------------------------------------------------
        // Goal      : Get the users' membership requests in the model.
        // Created by Miguel Santamario on 30.05.2015 13:35
        //----------------------------------------------------------------------------------
        QMap<quint32, ModelRequest> getRequests() const;

        //----------------------------------------------------------------------------------
        // Goal      : Get a user's membership request in the model.
        // Created by Miguel Santamario on 30.05.2015 14:16
        //----------------------------------------------------------------------------------
        ModelRequest& getRequest(const quint32 idRequest);

        void setRsaKeyPair(const RSAPair& rsaKeyPair);
        RSAPair getRsaKeyPair() const;
};


class ModelRoom
{
    friend QDataStream& operator<< (QDataStream& ds, const ModelRoom& r);
    friend QDataStream& operator>> (QDataStream& ds, ModelRoom& r);
    
	private :
    
        // Attributes of the room
        quint32 _idRoom;
        QString _name;
		bool _private;
        bool _visible;
        QImage _picture;
        quint32 _limitOfStoredMessage;
        
        // The admins and members of the room, only the ids are stocked.
        QSet<quint32> _admins;
        QSet<quint32> _members;
        
        // Room messages are stocked here.
        QMap<quint32, ModelMessage> _messages;

        // The private room's secret key.
        AESKey _secretKey;
	
	public :
        ModelRoom();
        
        ModelRoom(const quint32 idRoom, const QString& name, const quint32 limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QImage& picture, const QSet<quint32>& adminsIds, const QSet<quint32>& usersIds, const QMap<quint32, ModelMessage>& messages, const AESKey& secretKey = AESKey());
		~ModelRoom();

        //----------------------------------------------------------------------------------
        // Goal      : Add a message to the messages list.
        //             Used anytime we need to store a message in the application.
        // Created by Miguel Santamaria, on 19.04.2015 21:20
        // Last edited by Jan Purro, on 24.04.2015 15:45
        //----------------------------------------------------------------------------------
        void addMessage(const quint32 idMessage, const quint32 idRoom, const quint32 idUser, const QDateTime& date, const QDateTime &lastEditionDate, const QByteArray& content);
        void addMessage(const ModelMessage& message);


        //----------------------------------------------------------------------------------
        // Goal      : Modify a message.
        // Param     : idMessage - Message's identifier
        // Last edited by Miguel Santamaria, on 09.05.2015 18:47
        //----------------------------------------------------------------------------------
        void modifyMessage(const quint32 idMessage, const QByteArray& content, const QDateTime lastEditionDate);

        //----------------------------------------------------------------------------------
        // Goal      : Delete a message from the message's list.
        // Param     : idMessage - message's identifier
        //----------------------------------------------------------------------------------
        void deleteMessage(const quint32 idMessage);
		
        //----------------------------------------------------------------------------------
        // Goal      : Add a user to the user list.
        // Param     : idUser - User's identifier
        // Created by Jan Purro, on 24.04.2015 15:35
        // Last edited by Jan Purro, on 24.04.2015 15:35
        //----------------------------------------------------------------------------------
        void addUser(const quint32 idUser);

        //----------------------------------------------------------------------------------
        // Goal      : Remove the given user of the room..
        // Param     : idUser - User's identifier
        // Created by Miguel Santamaria on 14.05.2015 19:57
        //----------------------------------------------------------------------------------
        void removeUser(const quint32 userId);
        
        //----------------------------------------------------------------------------------
        // Goal      : Add an admin to the admin list AND the user list.
        // Param     : idUser - User's identifier
        // Created by Jan Purro, on 24.04.2015 15:35
        // Last edited by Jan Purro, on 24.04.2015 15:35
        //----------------------------------------------------------------------------------
        void addAdmin(const quint32 idUser);

        // Getters
        quint32 getIdRoom() const;
        QMap<quint32, ModelMessage>& getMessages();
        const QSet<quint32>& getUsers() const;
        const QSet<quint32>& getAdmins() const;
        const QString& getName() const;
        const QImage& getPicture() const;
        quint32 getLimit() const;
        bool isPrivate() const ;
        bool isVisible() const;
        AESKey& getSecretKey();
        ModelMessage& getMessage(const quint32 idMessage);

        // Setters
        void modifyRoom(const QString& name, const quint32 limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QImage& picture);
        void setIdRoom(const quint32 id);
        void setPicture(const QImage& picture);
        void setKey(const AESKey& aeskey);
};

class ModelMessage
{
    friend QDataStream& operator<< (QDataStream& ds, const ModelMessage& m);
    friend QDataStream& operator>> (QDataStream& ds, ModelMessage& m);

	private :
        quint32 _idMessage;
        quint32 _idRoom;
        quint32 _idUser;
        QDateTime _date;
        QDateTime _lastEditionDate;
        QByteArray _content;
	
	public :
        ModelMessage();
        ModelMessage(const quint32 idMessage, const quint32 idRoom, const quint32 idUser, const QDateTime& date, const QDateTime& lastEditionDate, const QByteArray& content);
		~ModelMessage();
		
        //----------------------------------------------------------------------------------
        // Goal      : Modify a message
        // Param     : content - New content
        //             lastEditionDate - Last date of modification
        //----------------------------------------------------------------------------------
        void modify(const QByteArray& content, const QDateTime lastEditionDate);

        // Getters
        quint32 getIdMessage() const;
        quint32 getIdUser() const;
        quint32 getIdRoom() const;
        QDateTime& getDate();
        QDateTime& getEditionDate();
        const QByteArray& getContent() const;
        
        // Setters
        void setIdMessage(const quint32 id);
        void setDate(const QDateTime& date);
        void setEditionDate(const QDateTime& date);
        void setContent(const QByteArray& content);
};

class ModelUser
{
    friend QDataStream& operator<< (QDataStream& ds, const ModelUser& u);
    friend QDataStream& operator>> (QDataStream& ds, ModelUser& u);
    
	private :
        quint32 _idUser;
        QString _userName;
        QString _firstName;
        QString _lastName;
        bool _isConnected;
        QDateTime _lastConnection;
        QImage _image;
        QSet<quint32> _roomsIds;
	
	public :
        ModelUser();
        ModelUser(const quint32 idUser, const QString& userName, const QString& firstName, const QString& lastName, const bool isConnected, const QDateTime& lastConnection, const QImage& image, const QSet<quint32> roomsIds);
		~ModelUser();
		
        //----------------------------------------------------------------------------------
        // Goal      : Modify a user's information
        // Param     : firstName - New first name
        //             lastName - New last Name
        //             image - New image
        //----------------------------------------------------------------------------------
        void modify(const QString& firstName, const QString& lastName, const QImage& image);

        // Getters
        quint32 getIdUser() const;
        const QString& getUserName()const ;
        const QString& getFirstName() const;
        const QString& getLastName() const;
        void setFirstName(const QString firstName);
        void setLastName(const QString lastName);
        bool isConnected() const;
        const QImage& getImage() const;
        const QSet<quint32>& getRooms() const;
        QSet<quint32>& getRooms();
        
        // Setters
        void setIdUser(const quint32 id);
        void setConnected(const bool connected);
        void setImage(const QImage& image);
        
};

class ModelRequest
{
    private :
        quint32 _id;
        ModelRoom _room;
        ModelUser _user;
        QByteArray _publicKey;

    public :
        ModelRequest();
        ModelRequest(const quint32 id, const ModelRoom& room, const ModelUser& user, const QByteArray& publicKey);
        ~ModelRequest();

        // Getters
        quint32 getId() const;
        ModelRoom getRoom() const;
        ModelUser getUser() const;
        QByteArray& getPublicKey();
};

QDataStream& operator<< (QDataStream& ds, const ModelRoom& r);
QDataStream& operator>> (QDataStream& ds, ModelRoom& r);
QDataStream& operator<< (QDataStream& ds, const ModelMessage& m);
QDataStream& operator>> (QDataStream& ds, ModelMessage& m);
QDataStream& operator<< (QDataStream& ds, const ModelUser& u);
QDataStream& operator>> (QDataStream& ds, ModelUser& u);
QDataStream& operator<< (QDataStream& ds, const vector<unsigned char>& v);
QDataStream& operator>> (QDataStream& ds, vector<unsigned char>& v);
QDataStream& operator<< (QDataStream& ds, const vector<char>& v);
QDataStream& operator>> (QDataStream& ds, vector<char>& v);
QDataStream& operator<< (QDataStream& ds, const RSAPair& p);
QDataStream& operator>> (QDataStream& ds, RSAPair& p);
QDataStream& operator<< (QDataStream& ds, const AESKey& k);
QDataStream& operator>> (QDataStream& ds, AESKey& k);

#endif // MODELCHATOR
