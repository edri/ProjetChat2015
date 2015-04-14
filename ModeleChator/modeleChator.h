//Envoyer tous les infos relatives à un user quand il rejoint une salle aux autres utilisateurs, également si ils conaissent déjà l'utilisateur en question

//Contain any informations related to the chat
class ModelChator
{
	private :
		list<ModelRoom>* rooms;
	
	public :
		ModelChator();
	
		ModeleRoom getRoom(const int idRoom) const;
		void addRoom(const int idRoom, const Qstring& name, const int limitOfStoredMessage, const bool isPrivate, const bool isVisible, const Qstring& picture);
		void modifyRoom(const int idRoom, const Qstring& name, const int limitOfStoredMessage, const bool isPrivate, const bool isVisible, const Qstring& picture);
		void deleteRoom(const int idRoom);
		
		ModeleMessage getMessage(const idRoom, const idMessage) const;
		void addMessage(const int idRoom, const int idMessage, const int idUser, const QDate& date, const QString& contents);
		void modifyMessage(const int idRoom, const int idMessage, const QString& contents);
		void deleteMessage(const int idRoom, const int idMessage);	
		
		ModeleUser addUser(const int idRoom, const int idUser, const QString userName, const Qstring& firstName, const QString& lastName, const bool isConnected, const QDate& lastConnection, const QString& image);		
		ModeleUser getUser(const int idRoom, const int idUser) const;
		
		ModeleUser addAdmin(const int idRoom, const int idUser);
}


class ModelRoom
{
	private :
		bool isPrivate;
		bool isVisible;
		list<ModelUser>* admins;
		list<Message>* messages;
		list<ModelUser>* users;
	
	public :
		ModelRoom(const int idRoom, const Qstring& name, const int limitOfStoredMessage, const bool isPrivate, const bool isVisible, const Qstring& picture, list<ModelUser>* admins);	
		ModelRoom(const int idRoom, const Qstring& name, const int limitOfStoredMessage, const bool isPrivate, const bool isVisible, const Qstring& picture, list<ModelUser>* admins, list<ModelUser>* users)
		~ModelRoom();
		
		int getIdRoom();
		void modifyRoom(const Qstring& name, const int limitOfStoredMessage, const bool isPrivate, const bool isVisible, const Qstring& picture);
		
		void addMessage(const int idMessage, const int idUser, const QDate& date, const QString& contents);
		void modifyMessage(const int idMessage, const QString& contents);
		void deleteMessage(const int idMessage);
		
		void addUser(const int idUser, const QString userName, const Qstring& firstName, const QString& lastName, const bool isConnected, const QDate& lastConnection, const QString& image);
		void addAdmin(const int idUser);
}

class ModelMessage
{
	private :
		int idMessage;
		int idUser;
		QDate date;
		QString contents;
	
	public :
		ModelMessage(const int idMessage, const int idUser, const QDate& date, const QString& contents);
		~ModelMessage();
		
		int getIdMessage();	
		void modify(const QString& contents);
}

class ModelUser
{
	private :
		int idUser;
		QString userName;
		QString firstName;
		QString lastName;
		bool isConnected;
		QDate lastConnection;
		QString image;
	
	public :
		ModelUser(const int idUser, const QString userName, const Qstring& firstName, const QString& lastName, const bool isConnected, const QDate& lastConnection, const QString& image);
		~ModeleUser();
		
		int getIdUser();		
		void modify(const Qstring& firstName, const QString& lastName, const QString& image);
}