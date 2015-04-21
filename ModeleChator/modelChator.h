//Contain any informations related to the chat
#ifndef MODELCHATOR
#define MODELCHATOR

#include <QtWidgets>
#include <QString>

using namespace std;

class ModelUser;
class ModelMessage;
class ModelRoom;

class ModelChator
{
	private :
        QMap<quint32, ModelRoom*> _rooms;
        QMap<quint32, ModelUser*> _users;
	
	public :
        ModelChator();

        /*
         * Created by Miguel Santamaria, on 17.04.2015 21:20
         *
         * Get all the given user's rooms, and return them in a QMap.
         * Used in : - ControllerChat::loadRooms.
         *
         * Last edited by Miguel Santamaria, on 17.04.2015 22:05
        */
        QMap<quint32, ModelRoom*> getUserRooms(const quint32 idUser) const;

        /*
         * Created by Miguel Santamaria, on 17.04.2015 21:59
         *
         * Add an user to the users list.
         * Used anytime we need to store an user in the application.
         *
         * Last edited by Miguel Santamaria, on 17.04.2015 21:59
        */
        void addUser(const quint32 idUser, const QString& userName, const QString& firstName, const QString& lastName, const bool isConnected, const QDateTime& lastConnection, const QString& image);
        void addUser(ModelUser* user);

        /*
         * Created by Miguel Santamaria, on 18.04.2015 00:18
         *
         * Add room to the rooms list.
         * Used anytime we need to store a room in the application.
         *
         * Last edited by Miguel Santamaria, on 18.04.2015 00:18
        */
        void addRoom(const quint32 idRoom, const QString& name, const quint32 limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QString& picture, QMap<quint32, ModelUser*>& admins, QMap<quint32, ModelUser*>& users);
        void addRoom(ModelRoom* room);

        /*
         * Created by Miguel Santamaria, on 20.04.2015 12:22
         *
         * Get the room identified by the given id.
         * Used anytime we need to get a room in the application.
         *
         * Last edited by Miguel Santamaria, on 20.04.2015 12:22
        */
        ModelRoom* getRoom(const quint32 idRoom) const;

        void modifyRoom(const quint32 idRoom, const QString& name, const quint32 limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QString& picture);
		void deleteRoom(const quint32 idRoom);
		
        ModelMessage getMessage(const quint32 idRoom, const quint32 idMessage) const;
        void modifyMessage(const quint32 idRoom, const quint32 idMessage, const QString& contents);
		void deleteMessage(const quint32 idRoom, const quint32 idMessage);	
		
        ModelUser getUser(const quint32 idRoom, const quint32 idUser) const;
		ModelUser addAdmin(const quint32 idRoom, const quint32 idUser);
};


class ModelRoom
{
    friend QDataStream& operator<< (QDataStream& ds, const ModelRoom& r);
    friend QDataStream& operator>> (QDataStream& ds, ModelRoom& r);
    
	private :
        quint32 _idRoom;
        QString _name;
		bool _private;
        bool _visible;
        QString _picture;
        quint32 _limitOfStoredMessage;
        QMap<quint32, ModelUser*> _admins;
        QMap<quint32, ModelMessage*> _messages;
        QMap<quint32, ModelUser*> _users;
	
	public :
        ModelRoom();
        ModelRoom(const quint32 idRoom, const QString& name, const quint32 limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QString& picture, QMap<quint32, ModelUser*>& admins);
        ModelRoom(const quint32 idRoom, const QString& name, const quint32 limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QString& picture, QMap<quint32, ModelUser*>& admins, QMap<quint32, ModelUser*>& users);
		~ModelRoom();

        /*
         * Created by Miguel Santamaria, on 19.04.2015 21:20
         *
         * Add a message to the messages list.
         * Used anytime we need to store a message in the application.
         *
         * Last edited by Miguel Santamaria, on 1+.04.2015 21:20
        */
        void addMessage(const quint32 idMessage, const quint32 idRoom, const quint32 idUser, const QDateTime& date, const QString& content);
        void addMessage(ModelMessage* message);

		void modifyMessage(const quint32 idMessage, const QString& contents);
		void deleteMessage(const quint32 idMessage);
		
        void addUser(const quint32 idUser, const QString& userName, const QString& firstName, const QString& lastName, const bool isConnected, const QDateTime& lastConnection, const QString& image);
		void addAdmin(const quint32 idUser);

        // Getters
        quint32 getIdRoom() const;
        QMap<quint32, ModelMessage*> getMessages() const;
        QMap<quint32, ModelUser*> getUsers() const;
        QString getName() const;
        QString getPicture() const;
        quint32 getLimit() const;
        bool isPrivate() const ;
        bool isVisible() const;

        // Setters
        void modifyRoom(const QString& name, const quint32 limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QString& picture);
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
        QString _content;
	
	public :
        ModelMessage();
        ModelMessage(const quint32 idMessage, const quint32 idRoom, const quint32 idUser, const QDateTime& date, const QString& content);
		~ModelMessage();
		
        void modify(const QString& content);

        // Getters
        quint32 getIdMessage() const;
        quint32 getIdUser() const;
        QDateTime getDate() const;
        QString getContent() const;
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
        QString _image;
	
	public :
        ModelUser();
        ModelUser(const quint32 idUser, const QString& userName, const QString& firstName, const QString& lastName, const bool isConnected, const QDateTime& lastConnection, const QString& image);
		~ModelUser();
		
        void modify(const QString& firstName, const QString& lastName, const QString& image);

        // Getters
        quint32 getIdUser() const;
        QString getUserName() const;
        bool isConnected() const;
        QString getImage() const;
};

QDataStream& operator<< (QDataStream& ds, const ModelRoom& r);
QDataStream& operator>> (QDataStream& ds, ModelRoom& r);
QDataStream& operator<< (QDataStream& ds, const ModelMessage& m);
QDataStream& operator>> (QDataStream& ds, ModelMessage& m);
QDataStream& operator<< (QDataStream& ds, const ModelUser& u);
QDataStream& operator>> (QDataStream& ds, ModelUser& u);

#endif // MODELCHATOR
