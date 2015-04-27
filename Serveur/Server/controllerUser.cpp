#include "controllerUser.h"
#include "../Interpretor/packet.h"

ControllerUser::ControllerUser(ControllerDB& db, ControllerRoom& room) : _db(db), _room(room) {}

void ControllerUser::login(const QString& pseudo, const QString& hashedPWD, ChatorClient* client)
{
    qDebug() << "tentative de login depuis " << client->socket.peerAddress().toString() << pseudo << ", " << hashedPWD;
    
    if (_db.login(pseudo, hashedPWD, client->id))
    {
        quint32 id = client->id;
        qDebug() << "Authentification OK, userid = " << id;
        
        // Send the ModelUser to the client
        client->logged = true;
        ModelUser user = _db.info(id);
        qDebug() << "User: " << user.getUserName();
        client->socket.sendBinaryMessage(interpretor->sendInfoUser(user));
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
                qDebug() << "Room " << idRoom << ", message " << mm.getIdMessage() << ": " << mm.getContent();
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
        
        client->socket.sendBinaryMessage(interpretor->join(rooms, users));
        
        // Inform everyone in the rooms that this client is online
        _room.userConnected(user, client);
    }
    else
    {
        qDebug() << "Erreur d'authentification";
        client->socket.sendBinaryMessage(interpretor->sendError(ModelError(ErrorType::AUTH_ERROR, "incorrect login or password")));
    }
}

void ControllerUser::userId(const QString& userName, ChatorClient* client)
{
    quint32 userId = 0;
    bool exists = _db.userExists(userName, userId);
    client->socket.sendBinaryMessage(interpretor->userId(userName, exists, userId));
}
