#include "controllerUser.h"
#include "../Interpretor/packet.h"

ControllerUser::ControllerUser(ControllerDB& db, ControllerRoom& room) : _db(db), _room(room) {}

void ControllerUser::login(const QString& pseudo, const QString& hashedPWD, ChatorClient* client)
{
    quint32 id = 0;
    if (_db.login(pseudo, hashedPWD, client->id))
    {
        client->logged = true;
        ModelUser user = _db.info(id);
        client->socket.sendBinaryMessage(interpretor->sendInfoUser(user));
        // Clés?
        // Informer les salles que cet utilisateur s'est connecté
    }
    else
    {
        client->socket.sendBinaryMessage(interpretor->sendError(ModelError(ErrorType::AUTH_ERROR, "incorrect login or password")));
    }
}
