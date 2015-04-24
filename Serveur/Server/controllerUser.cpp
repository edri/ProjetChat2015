#include "controllerUser.h"
#include "../Interpretor/packet.h"

ControllerUser::ControllerUser(ControllerDB& db, ControllerRoom& room) : _db(db), _room(room) {}

void ControllerUser::login(const QString& pseudo, const QString& hashedPWD, ChatorClient* client)
{
    if (_db.login(pseudo, hashedPWD, client->id))
    {
        client->logged = true;
        ModelUser user = _db.info(id);
        client->socket.sendBinaryMessage(interpretor->sendInfoUser(user));
        // Clés?
        
        QMap<quint32, ModeRoom> rooms;
        QMap<quint32, ModeUser> users;
        
        QSet<quint32> idRooms = user.getRooms();
        for (quint32 idRoom : idRooms)
        {
            ModelRoom room = _db.infoRoom(idRoom);
            rooms.insert(idRoom, _db.infoRoom(idRoom));
            QSet<quint32> roomUsers = room.getUsers();
            for (quint32 idUser : roomUsers)
            {
                if (!users.contains(idUser))
                {
                    users.insert(idUserm, _db.info(idUser));
                }
            }
        }
        
        client->socket.sendBinaryMessage(interpretor->join(rooms, users));
        
        // Informer les salles que cet utilisateur s'est connecté
    }
    else
    {
        client->socket.sendBinaryMessage(interpretor->sendError(ModelError(ErrorType::AUTH_ERROR, "incorrect login or password")));
    }
}
