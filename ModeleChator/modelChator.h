//Contain any informations related to the chat
#ifndef MODELCHATOR
#define MODELCHATOR

#include <QtWidgets>
#include <QString>

#include "../Cryptor/cryptor.h"

using namespace std;

class ModelUser;
class ModelMessage;
class ModelRoom;

class ModelChator
{
	private :
        // The room and users are stocked here. Messages are stocked in their
        // respective rooms.
        QMap<quint32, ModelRoom> _rooms;
        QMap<quint32, ModelUser> _users;

        RSAPair _rsaKeyPair;
	
	public :
    
        ModelChator();

        /*
         * Created by Miguel Santamaria, on 17.04.2015 21:20
         *
         * Get all the given user's rooms, and return them in a QMap.
         * Used in : - ControllerChat::loadRooms.
         *
         * Last edited by Jan Purro, on 24.04.2015 16:23
        */
        QList<quint32> getUserRooms(const quint32 idUser) const;

        /*
         * Created by Miguel Santamaria, on 17.04.2015 21:59
         *
         * Add an user to the users list.
         * Used anytime we need to store an user in the application.
         *
         * Last edited by Jan Purro, on 24.04.2015 15::57
        */
        void addUser(const quint32 idUser, const QString& userName, const QString& firstName, const QString& lastName, const bool isConnected, const QDateTime& lastConnection, const QImage& image, const QSet<quint32> roomsIds);
        void addUser(const ModelUser& user);
        
        /*
         * Created by ?? on ??
         *
         * Add an admin to the specified room
         *
         * Last edited by Jan Purro, on 24.04.2015 16:31
        */
        void addAdmin(const quint32 idRoom, const quint32 idUser);
        
        /*
         * Created by Miguel Santamaria, on 21.04.2015 09:12
         *
         * Get the user identified by the given id.
         * Used anytime we need to get an user in the application.
         *
         * Last edited by Jan Purro, on 24.04.2015 16:07
        */
        ModelUser& getUser(const quint32 idUser);
        const ModelUser& getUser(const quint32 idUser) const;


        /*
         * Created by Miguel Santamaria, on 18.04.2015 00:18
         *
         * Add room to the rooms list.
         * Used anytime we need to store a room in the application.
         *
         * Last edited by Miguel Santamaria, on 18.04.2015 00:18
        */
        void addRoom(const quint32 idRoom, const QString &name, const quint32 limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QImage &picture, const QSet<quint32>& admins, const QSet<quint32>& users, const QMap<quint32, ModelMessage>& messages);
        void addRoom(const ModelRoom& room);
        /*
         * Created by Miguel Santamaria, on 20.04.2015 12:22
         *
         * Get the room identified by the given id.
         * Used anytime we need to get a room in the application.
         *
         * Last edited by Jan Purro, on 24.04.2015 16:11
        */
        ModelRoom& getRoom(const quint32 idRoom);
        const ModelRoom& getRoom(const quint32 idRoom) const;
        
        // Last edited by Jan Purro on 24.04.2015 16:40
        void modifyRoom(const quint32 idRoom, const QString& name, const quint32 limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QImage& picture);
		void deleteRoom(const quint32 idRoom);

        /*
         * Created by Miguel Santamaria on 14.05.2015 19:55
         *
         * Unsuscribe the given user to the given room.
         *
         */
        void removeUser(const quint32 userId, const quint32 roomId);
		
        // Last edited by Jan Purro on 24.04.2015 16:47
        ModelMessage& getMessage(const quint32 idRoom, const quint32 idMessage);
        // Last edited by Miguel Santamaria on 09.05.2015 18:46
        // Adding last edition's date.
        void modifyMessage(const quint32 idRoom, const quint32 idMessage, const QByteArray& content, const QDateTime lastEditionDate);
        void deleteMessage(const quint32 idRoom, const quint32 idMessage);

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
        //QString _picture;
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
        
        ModelRoom(const quint32 idRoom, const QString& name, const quint32 limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QImage& picture, const QSet<quint32>& adminsIds, const QSet<quint32>& usersIds, const QMap<quint32, ModelMessage>& messages);
		~ModelRoom();

        /*
         * Created by Miguel Santamaria, on 19.04.2015 21:20
         *
         * Add a message to the messages list.
         * Used anytime we need to store a message in the application.
         *
         * Last edited by Jan Purro, on 24.04.2015 15:45
        */
        void addMessage(const quint32 idMessage, const quint32 idRoom, const quint32 idUser, const QDateTime& date, const QDateTime &lastEditionDate, const QByteArray& content);
        void addMessage(const ModelMessage& message);
        
        // Last edited by Miguel Santamaria, on 09.05.2015 18:47
        // Adding the last edition's date.
        void modifyMessage(const quint32 idMessage, const QByteArray& content, const QDateTime lastEditionDate);
		void deleteMessage(const quint32 idMessage);
		
        /*
         * Created by Jan Purro, on 24.04.2015 15:35
         * 
         * Add a user to the user list
         * 
         * Last edited by Jan Purro, on 24.04.2015 15:35
         */
        void addUser(const quint32 idUser);

        /*
         * Created by Miguel Santamaria on 14.05.2015 19:57
         *
         * Remove the given user of the room.
         */
        void removeUser(const quint32 userId);
        
        /*
         * Created by Jan Purro, on 24.04.2015 15:35
         * 
         * Add an admin to the admin list AND the user list.
         * 
         * Last edited by Jan Purro, on 24.04.2015 15:35
         */
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
		
        void modify(const QString& firstName, const QString& lastName, const QImage& image);

        // Getters
        quint32 getIdUser() const;
        const QString& getUserName()const ;
        const QString& getFirstName() const;
        const QString& getLastName() const;
        bool isConnected() const;
        const QImage& getImage() const;
        const QSet<quint32>& getRooms() const;
        QSet<quint32>& getRooms();
        
        // Setters
        void setIdUser(const quint32 id);
        void setConnected(const bool connected);
        void setImage(const QImage& other);
        
};

QDataStream& operator<< (QDataStream& ds, const ModelRoom& r);
QDataStream& operator>> (QDataStream& ds, ModelRoom& r);
QDataStream& operator<< (QDataStream& ds, const ModelMessage& m);
QDataStream& operator>> (QDataStream& ds, ModelMessage& m);
QDataStream& operator<< (QDataStream& ds, const ModelUser& u);
QDataStream& operator>> (QDataStream& ds, ModelUser& u);

#endif // MODELCHATOR
