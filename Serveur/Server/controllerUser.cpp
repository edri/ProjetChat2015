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
        
        // Envoi du ModelUser
        client->logged = true;
        ModelUser user = _db.info(id);
        qDebug() << "User: " << user.getUserName();
        client->socket.sendBinaryMessage(interpretor->sendInfoUser(user));
        // Clés?
        
        QMap<quint32, ModelRoom> rooms;
        QMap<quint32, ModelUser> users;
        
        // Récupération des ids des salles jointes
        QSet<quint32> idRooms = user.getRooms();
        for (quint32 idRoom : idRooms)
        {
            // Récupération des informations de chaque salle
            ModelRoom room = _db.infoRoom(idRoom);
            rooms.insert(idRoom, _db.infoRoom(idRoom));
            
            // Récupération des ids des utilisateurs de chaque salle
            QSet<quint32> roomUsers = room.getUsers();
            for (quint32 idUser : roomUsers)
            {
                // Si cet utilisateur n'est pas encore dans la liste, on le met
                if (!users.contains(idUser))
                {
                    // Récupération des informations de l'utilisateur
                    users.insert(idUser, _db.info(idUser));
                }
            }
        }
        
        client->socket.sendBinaryMessage(interpretor->join(rooms, users));
        
        // Informer les salles que cet utilisateur s'est connecté
        _room.userConnected(user, client);
    }
    else
    {
        qDebug() << "Erreur d'authentification";
        client->socket.sendBinaryMessage(interpretor->sendError(ModelError(ErrorType::AUTH_ERROR, "incorrect login or password")));
    }
}
