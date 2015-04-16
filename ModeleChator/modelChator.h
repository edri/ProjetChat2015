//Contain any informations related to the chat

#include <QtWidgets>
#include <QString>
#include <QMap>
#include <QList>

using namespace std;

class ModelUser;
class ModelMessage;
class ModelRoom;

class ModelChator
{
	private :
		QMap<quint32, ModelRoom> rooms;
		QMap<quint32, ModelUser> users;
	
	public :
		ModelChator();
	
		ModelRoom getRoom(const quint32 idRoom) const;
		void addRoom(const quint32 idRoom, const QString& name, const quint32 limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QString& picture);
		void modifyRoom(const quint32 idRoom, const QString& name, const quint32 limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QString& picture);
		void deleteRoom(const quint32 idRoom);
		
		ModelMessage getMessage(const quint32 idRoom, const quint32 idMessage) const;
		void addMessage(const quint32 idRoom, const quint32 idMessage, const quint32 idUser, const QDateTime& date, const QString& contents);
		void modifyMessage(const quint32 idRoom, const quint32 idMessage, const QString& contents);
		void deleteMessage(const quint32 idRoom, const quint32 idMessage);	
		
		ModelUser addUser(const quint32 idRoom, const quint32 idUser, const QString userName, const QString& firstName, const QString& lastName, const bool isConnected, const QDateTime& lastConnection, const QString& image);		
		ModelUser getUser(const quint32 idRoom, const quint32 idUser) const;
		
		ModelUser addAdmin(const quint32 idRoom, const quint32 idUser);
};


class ModelRoom
{
    friend QDataStream& operator<< (QDataStream& ds, const ModelRoom& r);
    friend QDataStream& operator>> (QDataStream& ds, ModelRoom& r);
    
	private :
		quint32 idRoom;
		bool isPrivate;
		bool isVisible;
		QList<ModelUser*> admins;
		QList<ModelMessage> messages;
		QList<ModelUser*> users;
	
	public :
		ModelRoom(const quint32 idRoom, const QString& name, const quint32 limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QString& picture, QMap<quint32, ModelUser>* admins);	
		ModelRoom(const quint32 idRoom, const QString& name, const quint32 limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QString& picture, QMap<quint32, ModelUser>* admins, QMap<quint32, ModelUser>* users);
		~ModelRoom();
		
		int getIdRoom();
		void modifyRoom(const QString& name, const quint32 limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QString& picture);
		
		void addMessage(const quint32 idMessage, const quint32 idUser, const QDateTime& date, const QString& contents);
		void modifyMessage(const quint32 idMessage, const QString& contents);
		void deleteMessage(const quint32 idMessage);
		
		void addUser(const quint32 idUser, const QString userName, const QString& firstName, const QString& lastName, const bool isConnected, const QDateTime& lastConnection, const QString& image);
		void addAdmin(const quint32 idUser);
		QMap<quint32, ModelUser>* getUsers();
};

class ModelMessage
{
    friend QDataStream& operator<< (QDataStream& ds, const ModelMessage& m);
    friend QDataStream& operator>> (QDataStream& ds, ModelMessage& m);

	private :
		quint32 idMessage;
		quint32 idUser;
		QDateTime date;
		QString content;
	
	public :
        ModelMessage();
		ModelMessage(const quint32 idMessage, const quint32 idUser, const QDateTime& date, const QString& contents);
		~ModelMessage();
		
		int getIdMessage();	
		void modify(const QString& contents);
};

class ModelUser
{
    friend QDataStream& operator<< (QDataStream& ds, const ModelUser& u);
    friend QDataStream& operator>> (QDataStream& ds, ModelUser& u);
    
	private :
		quint32 idUser;
		QString userName;
		QString firstName;
		QString lastName;
		bool isConnected;
		QDateTime lastConnection;
		QImage image;
	
	public :
        ModelUser();
		ModelUser(const quint32 idUser, const QString userName, const QString& firstName, const QString& lastName, const bool isConnected, const QDateTime& lastConnection, const QImage& image);
		~ModelUser();
		
		int getIdUser();		
		void modify(const QString& firstName, const QString& lastName, const QImage& image);
};

QDataStream& operator<< (QDataStream& ds, const ModelRoom& r);
QDataStream& operator>> (QDataStream& ds, ModelRoom& r);
QDataStream& operator<< (QDataStream& ds, const ModelMessage& m);
QDataStream& operator>> (QDataStream& ds, ModelMessage& m);
QDataStream& operator<< (QDataStream& ds, const ModelUser& u);
QDataStream& operator>> (QDataStream& ds, ModelUser& u);
