#include "controllerRoom.h"

ControllerRoom::ControllerRoom(ControllerDB& db) : _db(db) {}

void ControllerRoom::storeMessage(ModelMessage& message, ChatorClient* client)
{
    if (client->logged && message.getIdUser() == client->id)
    {
        message.setIdMessage(_db.storeMessage(message));
        message.setDate(QDateTime::currentDateTime());
        ChatorRoom* room = onlineRooms[message.getIdRoom()];
        
        for (ChatorClient* client : room->clients)
        {
            client->socket.sendBinaryMessage(interpretor->sendMessage(message));
        }
    }
}

void ControllerRoom::userConnected(const ModelUser& user, ChatorClient* currentClient)
{    
    // Récupération des ids des salles auxquelles l'utilisateur est inscrit
    QSet<quint32> rooms = user.getRooms();
    
    for (quint32 idRoom : rooms)
    {
        // On vérifie si cette salle est en ligne
        ChatorRoom* currentRoom;
        
        if (!onlineRooms.contains(idRoom))
        {
            // Si elle n'est pas en ligne on la crée
            currentRoom = new ChatorRoom({currentClient->id, {}});
            onlineRooms.insert(idRoom, currentRoom);
        }
        else
        {
            // Sinon, on récupère son pointeur
            currentRoom = onlineRooms[idRoom];
        }
        
        // On stocke le pointeur dans l'utilisateur
        currentClient->rooms.append(currentRoom);
        currentRoom->clients.append(currentClient);
    }
    
    for (ChatorRoom* room : currentClient->rooms)
    {
        for (ChatorClient* client : room->clients)
        {
            client->socket.sendBinaryMessage(interpretor->connected(user));
            //client->socket.sendBinaryMessage(interpretor->join(currentClient.id, room->id));
        }
    }
}
