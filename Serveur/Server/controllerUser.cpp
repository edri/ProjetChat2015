/*
     * implements controllerUser.h
*/

#include "controllerUser.h"
#include "../Interpretor/packet.h"

ControllerUser::ControllerUser(ControllerDB& db) : _db(db) {}

void ControllerUser::login(const QString& pseudo, const QByteArray& hashedPWD, ChatorClient* client)
{    
    if (_db.login(pseudo, hashedPWD, client->id))
    {
        quint32 id = client->id;
        _connectedUsers.insert(id, client);
        
        // Send the ModelUser to the client
        client->logged = true;
        ModelUser user = _db.info(id);
        qDebug() << "Successful login from " << client->socket.peerAddress().toString() << ": " << user.getUserName();
        
        // We have to get his keyring and send it
        QByteArray keySalt;
        QByteArray publicKey;
        QByteArray privateKey;
        _db.getCryptoData(id, keySalt, publicKey, privateKey);
        client->socket.sendBinaryMessage(_interpretor->sendInfoUser(user, keySalt, privateKey, publicKey));
        
        // We have to get every ModelRoom and every ModelUser with whom this user is in relation
        QMap<quint32, ModelRoom> rooms;
        QMap<quint32, ModelUser> users;
        QList<QPair<quint32, QList<QPair<quint32, QByteArray>>>> requests;
        
        // Get the ids of the rooms of the user
        QSet<quint32>& idRooms = user.getRooms();
        for (quint32 idRoom : idRooms)
        {
            // Get the informations of each room
            ModelRoom room = _db.infoRoom(idRoom);
            
            // If the user is admin of the room, we have to send him the requests
            if (room.getAdmins().contains(id))
            {
                requests.append(QPair<quint32, QList<QPair<quint32, QByteArray>>>(idRoom, _db.getRequests(idRoom)));
            }
            
            // We get the AES key for this room and insert it into the ModelRoom
            QByteArray aesKeyAndIV = _db.getAesKey(user.getIdUser(), idRoom);
            QDataStream stream(&aesKeyAndIV, QIODevice::ReadOnly);
            
            AESKey aesKey;
            stream >> aesKey;
            room.setKey(aesKey);
            
            rooms.insert(idRoom, room);
            
            // Get the ids of all the users present in this room 
            QSet<quint32> roomUsers = room.getUsers();
            for (quint32 idUser : roomUsers)
            {
                // If this user isn't in the list yet, we insert it
                if (!users.contains(idUser))
                {
                    // Get the informations of the user
                    users.insert(idUser, _db.info(idUser));
                }
            }
        }
        
        // We can send the join
        client->socket.sendBinaryMessage(_interpretor->join(rooms, users));
        
        // Inform everyone in the rooms that this client is online
        _room->userConnected(user, client);
        
        // We can now send the requests for every room where this user is admin
        for (QPair<quint32, QList<QPair<quint32, QByteArray>>>& requestsForRoom : requests)
        {
            for (QPair<quint32, QByteArray>& r : requestsForRoom.second)
            {
                client->socket.sendBinaryMessage(_interpretor->request(requestsForRoom.first, _db.info(r.first), r.second, false));
            }
        }
    }
    
    // The user failed to login
    else
    {
        qDebug() << "Authentification failure from " << client->socket.peerAddress().toString();
        
        // We send him an error
        client->socket.sendBinaryMessage(_interpretor->sendError(ModelError(ErrorType::AUTH_ERROR, "Incorrect login informations or the user is already connected")));
    }
}

void ControllerUser::userId(const QString& userName, ChatorClient* client)
{
    quint32 userId = 0;
    bool exists = _db.userExists(userName, userId);
    client->socket.sendBinaryMessage(_interpretor->userId(userName, exists, userId));
}

void ControllerUser::createAccount(ModelUser& user, const QByteArray& password, const QByteArray& passwordSalt, const QByteArray& keySalt, const QByteArray& privateKey, const QByteArray& publicKey, ChatorClient* client)
{
    if (!_db.createAccount(user, password, passwordSalt, keySalt, privateKey, publicKey))
    {
        qDebug() << "Account creation failed for peer " << client->socket.peerAddress().toString() << ": " << user.getUserName();
        
        client->socket.sendBinaryMessage(_interpretor->sendError(ModelError(ErrorType::USER_CREATION, "Cannot create the user")));
        return;
    }
    
    qDebug() << "New account for peer " << client->socket.peerAddress().toString() << ": " << user.getUserName();

    // We automatically make the login
    login(user.getUserName(), password, client);
}

void ControllerUser::disconnect(ChatorClient* client)
{
    qDebug() << "The peer goes offline " << client->socket.peerAddress().toString();

    // Process the logout in the database (toggle the online bool, update the last connection date)
    _db.logout(client->id);
    
    // Remove the client from the list of all connected clients
    _connectedUsers.remove(client->id);
    
    // We have to notify every related clients that this one is disconnecting
    QSet<ChatorClient*> users;
    
    for (ChatorRoom* room : client->rooms)
    {
        // We remove this client from every room which he is in
        room->clients.remove(client);
        
        // Every other users of this room will have to be informed. We use a set so we can add multiple times a same client.
        users += room->clients;
        
        // If the room is now empty, this client was the last one so we have to disconnect the room
        if (room->clients.empty())
        {
            _room->_onlineRooms.remove(room->id);
            delete room;
        }
    }
    
    // Build the disconnect message
    QByteArray data = _interpretor->disconnect(client->id);
    
    // Send the disconnect message
    for (ChatorClient* user : users)
    {
        user->socket.sendBinaryMessage(data);
    }
}

void ControllerUser::modifyUser(const ModelUser& user, const QByteArray& password, const QByteArray& privateKey, ChatorClient* client)
{
    // We check that the peer is not trying to forge a modification request
    if (user.getIdUser() == client->id)
    {
        client->socket.sendBinaryMessage(_interpretor->sendError(ModelError(ErrorType::AUTH_ERROR, "incorrect user")));
        return;
    }
    
    // We actualy commit the change in the database
    _db.modifyUser(user, password, privateKey);
    
    QSet<quint32> upToDateClients;
    QByteArray data = _interpretor->editAccount(user);
    
    // We have to tell every other member who know him that the profile has changed
    for (ChatorRoom* currentRoom : client->rooms)
    {
        for (ChatorClient* currentClient : currentRoom->clients)
        {
            if (!upToDateClients.contains(currentClient->id))
            {
                currentClient->socket.sendBinaryMessage(data);
                upToDateClients.insert(currentClient->id);
            }
        }
    }
}

void ControllerUser::getSalt(const QString& pseudo, ChatorClient* client)
{
    QByteArray salt = _db.getSalt(pseudo);
    
    if (salt.isEmpty())
    {
        client->socket.sendBinaryMessage(_interpretor->sendError(ModelError(ErrorType::AUTH_ERROR, "incorrect login or password")));
    }
    else
    {
        client->socket.sendBinaryMessage(_interpretor->salt(pseudo, salt));
    }
}

void ControllerUser::getPublicKeys(QList<QPair<quint32, QByteArray>>& usersIdAndKey, ChatorClient* client)
{
    // We have to get the public key for every userID and send it back
    for (QPair<quint32, QByteArray>& userIdAndKey : usersIdAndKey)
    {
        userIdAndKey.second = _db.getPublicKey(userIdAndKey.first);
    }
    
    client->socket.sendBinaryMessage(_interpretor->publicKey(usersIdAndKey));
}

void ControllerUser::getAvalaibleRooms(ChatorClient* client)
{
    client->socket.sendBinaryMessage(_interpretor->listRooms(_db.listPublicRooms(), _db.listPrivateVisibleRooms()));
}
