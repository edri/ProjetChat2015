//Contain any informations related to the chat

#include <QtWidgets>
#include <QString>
#include <list>

using namespace std;

class ModelUser;
class ModelMessage;
class ModelRoom;

class ModelChator
{
	private :
		list<ModelRoom*> rooms;
	
	public :
		ModelChator();
	
		ModelRoom getRoom(const int idRoom) const;
		void addRoom(const int idRoom, const QString& name, const int limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QString& picture);
		void modifyRoom(const int idRoom, const QString& name, const int limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QString& picture);
		void deleteRoom(const int idRoom);
		
		ModelMessage getMessage(const quint32 idRoom, const quint32 idMessage) const;
		void addMessage(const int idRoom, const int idMessage, const int idUser, const QDateTime& date, const QString& contents);
		void modifyMessage(const int idRoom, const int idMessage, const QString& contents);
		void deleteMessage(const int idRoom, const int idMessage);	
		
		ModelUser addUser(const int idRoom, const int idUser, const QString userName, const QString& firstName, const QString& lastName, const bool isConnected, const QDateTime& lastConnection, const QString& image);		
		ModelUser getUser(const int idRoom, const int idUser) const;
		
		ModelUser addAdmin(const int idRoom, const int idUser);
};


class ModelRoom
{
	private :
		quint32 idRoom;
		bool isPrivate;
		bool isVisible;
		list<ModelUser>* admins;
		list<ModelMessage>* messages;
		list<ModelUser>* users;
	
	public :
		ModelRoom(const int idRoom, const QString& name, const int limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QString& picture, list<ModelUser>* admins);	
		ModelRoom(const int idRoom, const QString& name, const int limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QString& picture, list<ModelUser>* admins, list<ModelUser>* users);
		~ModelRoom();
		
		int getIdRoom();
		void modifyRoom(const QString& name, const int limitOfStoredMessage, const bool isPrivate, const bool isVisible, const QString& picture);
		
		void addMessage(const int idMessage, const int idUser, const QDateTime& date, const QString& contents);
		void modifyMessage(const int idMessage, const QString& contents);
		void deleteMessage(const int idMessage);
		
		void addUser(const int idUser, const QString userName, const QString& firstName, const QString& lastName, const bool isConnected, const QDateTime& lastConnection, const QString& image);
		void addAdmin(const int idUser);
};

class ModelMessage
{
	private :
		quint32 idMessage;
		quint32 idUser;
		QDateTime date;
		QString contents;
	
	public :
		ModelMessage(const int idMessage, const int idUser, const QDateTime& date, const QString& contents);
		~ModelMessage();
		
		int getIdMessage();	
		void modify(const QString& contents);
};

class ModelUser
{
	private :
		quint32 idUser;
		QString userName;
		QString firstName;
		QString lastName;
		bool isConnected;
		QDateTime lastConnection;
		QString image;
	
	public :
		ModelUser(const int idUser, const QString userName, const QString& firstName, const QString& lastName, const bool isConnected, const QDateTime& lastConnection, const QString& image);
		~ModelUser();
		
		int getIdUser();		
		void modify(const QString& firstName, const QString& lastName, const QString& image);
};
