#include "controllerUser.h"
#include "../Interpretor/packet.h"

ControllerUser::ControllerUser(ControllerDB& db) : _db(db) {}

void ControllerUser::login(const QString& pseudo, const QString& hashedPWD, ChatorClient* client)
{
    qDebug() << "tentative de login depuis " << client->socket.peerAddress().toString() << pseudo << ", " << hashedPWD;
    
    if (_db.login(pseudo, hashedPWD, client->id))
    {
        quint32 id = client->id;
        qDebug() << "Authentification OK, userid = " << id;
        
        _connectedUsers.insert(id, client);
        
        // Send the ModelUser to the client
        client->logged = true;
        ModelUser user = _db.info(id);
        qDebug() << "User: " << user.getUserName();
        client->socket.sendBinaryMessage(_interpretor->sendInfoUser(user));
        // Clés?
        
        QMap<quint32, ModelRoom> rooms;
        QMap<quint32, ModelUser> users;
        
        // Get the ids of the rooms of the user
        QSet<quint32> idRooms = user.getRooms();
        for (quint32 idRoom : idRooms)
        {
            qDebug() << "present dans salle: " << idRoom;
            // Get the informations of each room
            ModelRoom room = _db.infoRoom(idRoom);
            rooms.insert(idRoom, _db.infoRoom(idRoom));
            
            
            // Test
            QMap<quint32, ModelMessage> mes = room.getMessages();
            for (ModelMessage mm : mes) 
            {
                qDebug() << "Room " << idRoom << ", message " << mm.getIdMessage() << ": " << mm.getContent() << ", créé le " << mm.getDate().toString() << ", edité le " << mm.getEditionDate().toString();
             }
            
            // Get the ids of all the users present in this room 
            QSet<quint32> roomUsers = room.getUsers();
            for (quint32 idUser : roomUsers)
            {
                qDebug() << "autres present dans salle: " << idUser;
                // If this user isn't in the list yet, we insert it
                if (!users.contains(idUser))
                {
                    // Get the informations of the user
                    users.insert(idUser, _db.info(idUser));
                }
            }
        }
        
        for (ModelUser m : users)
        {
            qDebug() << "dans la liste: " << m.getUserName();
        }
        
        client->socket.sendBinaryMessage(_interpretor->join(rooms, users));
        
        // Inform everyone in the rooms that this client is online
        _room->userConnected(user, client);
    }
    else
    {
        qDebug() << "Erreur d'authentification";
        client->socket.sendBinaryMessage(_interpretor->sendError(ModelError(ErrorType::AUTH_ERROR, "incorrect login or password")));
    }
}

void ControllerUser::userId(const QString& userName, ChatorClient* client)
{
    quint32 userId = 0;
    bool exists = _db.userExists(userName, userId);
    client->socket.sendBinaryMessage(_interpretor->userId(userName, exists, userId));
}

QMap<quint32, ChatorClient*>& ControllerUser::getConnectedUsers()
{
    return _connectedUsers;
}

void ControllerUser::createAccount(ModelUser& user, ChatorClient* client)
{
    qDebug() << "Enregistrement";
    
    if (!_db.createAccount(user))
    {
        client->socket.sendBinaryMessage(_interpretor->sendError(ModelError(ErrorType::USER_CREATION, "Cannot create the user")));
        return;
    }
    
    // Appeler login
    //login(user.);
}

void ControllerUser::disconnect(ChatorClient* client)
{
    _db.logout(client->id);
    _connectedUsers.remove(client->id);
    
    QSet<ChatorClient*> users;
    
    for (ChatorRoom* room : client->rooms)
    {
        room->clients.remove(client);
        users += room->clients;
        if (room->clients.empty())
        {
            _room->getOnlineRooms().remove(room->id);
            delete room;
        }
    }
    
    QByteArray data = _interpretor->disconnect(client->id);
    
    for (ChatorClient* user : users)
    {
        user->socket.sendBinaryMessage(data);
    }
    
    delete client;
}
