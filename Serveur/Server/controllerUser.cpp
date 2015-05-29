#include "controllerUser.h"
#include "../Interpretor/packet.h"

ControllerUser::ControllerUser(ControllerDB& db) : _db(db) {}

void ControllerUser::login(const QString& pseudo, const QByteArray& hashedPWD, ChatorClient* client)
{
    qDebug() << "tentative de login depuis " << client->socket.peerAddress().toString() << ", pseudo: " << pseudo << " avec hash: " << QString::fromUtf8(hashedPWD.toHex());
    
    if (_db.login(pseudo, hashedPWD, client->id))
    {
        quint32 id = client->id;
        qDebug() << "Authentification OK, userid = " << id;
        
        _connectedUsers.insert(id, client);
        
        // Send the ModelUser to the client
        client->logged = true;
        ModelUser user = _db.info(id);
        qDebug() << "User: " << user.getUserName();
        // Clés?
        
        QByteArray keySalt;
        QByteArray publicKey;
        QByteArray privateKey;
        _db.getCryptoData(id, keySalt, publicKey, privateKey);
        
        client->socket.sendBinaryMessage(_interpretor->sendInfoUser(user, keySalt, privateKey, publicKey));
        
        QMap<quint32, ModelRoom> rooms;
        QMap<quint32, ModelUser> users;
        
        // Get the ids of the rooms of the user
        QSet<quint32> idRooms = user.getRooms();
        for (quint32 idRoom : idRooms)
        {
            // Test
            qDebug() << "present dans salle: " << idRoom;
            // until here
            
            // Get the informations of each room
            ModelRoom room = _db.infoRoom(idRoom);
            
            QByteArray aesKeyAndIV = _db.getAesKey(user.getIdUser(), idRoom);
            qDebug() << "La clé de la salle " << idRoom << " fait " << aesKeyAndIV.size();
            QDataStream stream(&aesKeyAndIV, QIODevice::ReadOnly);
            
            AESKey aesKey;
            stream >> aesKey;
            qDebug() << "L'aeskey contient: " << aesKey.key.size() << "|" << aesKey.initializationVector.size();
            room.setKey(aesKey);
            
            rooms.insert(idRoom, room);
            
            
            // Test
            QMap<quint32, ModelMessage> mes = room.getMessages();
            for (ModelMessage mm : mes) 
            {
                qDebug() << "Room " << idRoom << ", message " << mm.getIdMessage() << ": " << mm.getContent() << ", créé le " << mm.getDate().toString() << ", edité le " << mm.getEditionDate().toString();
            }// until here
            
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
        
        // Tests
        for (ModelUser m : users)
        {
            qDebug() << "dans la liste: " << m.getUserName();
        }// until here
        
        client->socket.sendBinaryMessage(_interpretor->join(rooms, users));
        
        // Inform everyone in the rooms that this client is online
        _room->userConnected(user, client);
    }
    else
    {
        // Test
        qDebug() << "Erreur d'authentification";
        // until here
        client->socket.sendBinaryMessage(_interpretor->sendError(ModelError(ErrorType::AUTH_ERROR, "incorrect login or password")));
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
    qDebug() << "Enregistrement de " << user.getUserName() << ", avec hash: " << QString::fromUtf8(password.toHex()) << ", avec sel: " << QString::fromUtf8(passwordSalt.toHex()) ;//<< ", avec clé privée : " << QString::fromUtf8(privateKey.toHex()) ;
    
    if (!_db.createAccount(user, password, passwordSalt, keySalt, privateKey, publicKey))
    {
        client->socket.sendBinaryMessage(_interpretor->sendError(ModelError(ErrorType::USER_CREATION, "Cannot create the user")));
        return;
    }

    // Appeler login
    login(user.getUserName(), password, client);
}

void ControllerUser::disconnect(ChatorClient* client)
{
    qDebug() << "Déconnexion...";
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
            qDebug() << "Suppression salle " << room->id << " car dernier utilisateur";
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
    
    qDebug() << "Fin Déconnexion...";
}

void ControllerUser::modifyUser(const ModelUser& user, const QByteArray& password, const QByteArray& privateKey, ChatorClient* client)
{
    if (user.getIdUser() == client->id)
    {
        client->socket.sendBinaryMessage(_interpretor->sendError(ModelError(ErrorType::AUTH_ERROR, "incorrect user")));
        return;
    }
    
    _db.modifyUser(user, password, privateKey);
    QSet<quint32> upToDateClients;
    QByteArray data = _interpretor->editAccount(user);
    
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
        qDebug() << "Envoi du sel pour " << pseudo << ": " << QString::fromUtf8(salt.toHex());
        client->socket.sendBinaryMessage(_interpretor->salt(pseudo, salt));
    }
}

void ControllerUser::getPublicKeys(QList<QPair<quint32, QByteArray>>& usersIdAndKey, ChatorClient* client)
{
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
