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
		
		ModeleUser getUser(const int idRoom, const int idUser) const;
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
	
		void modifyRoom(const Qstring& name, const int limitOfStoredMessage, const bool isPrivate, const bool isVisible, const Qstring& picture);
		void addMessage(const int idMessage, const int idUser, const QDate& date, const QString& contents);
		void modifyMessage(const int idMessage, const QString& contents);
		void deleteMessage(const int idMessage);
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
		
		void modify(const int idMessage, const QString& contents);
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
		ModelUser(int idUser, QString userName, QString lastName, bool isConnected, QDate lastConnection, QString image);
		~ModeleUser();
		
}