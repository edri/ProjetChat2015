#include "controllerUser.h"
#include "../Interpretor/packet.h"

ControllerUser::ControllerUser(ControllerDB& db) : _db(db) {}

void ControllerUser::login(const QString& pseudo, const QString& hashedPWD, ChatorClient* client)
{
    quint32 id = 0;
    if (_db.login(pseudo, hashedPWD, client->id))
    {
        client->logged = true;
        ModelUser user = _db.info(id);
        client->socket.sendBinaryMessage(interpretor->sendInfoUser(user));
    }
    else
    {
        client->socket.sendBinaryMessage(interpretor->sendError(ModelError(ErrorType::AUTH_ERROR, "incorrect login or password")));
    }
}
