//Contain any informations related to the chat
class ModelChator
{
	private :
		list<Salle>* salles;
	
	public :
		void getRoom(const int idRoom) const;
		void addRoom(const int idRoom, const Qstring& name, const int limitOfStoredMessage, const bool isPrivate, const bool isVisible, const Qstring& picture);
		void modifyRoom(const int idRoom, const Qstring& name, const int limitOfStoredMessage, const bool isPrivate, const bool isVisible, const Qstring& picture);
		void deleteRoom(int idRoom);
		void addMessage(int idRoom, int idMessage, const QString& contents)
		void deleteMessage(int idRoom, int idMessage)
}


class ModelRoom
{
	private :
		isPrivate;
		isVisible;
		list<Users> admins;
		list<Message> messages;
		list<User>* users;
	
	public :
		addMessage
		deleteMessage
}

class ModelMessage
{
	private :
		String user;
		String message;
		Date date;
	
	public :
		delete;
}

class ModelUser
{
	private :
	
	public :
		
}