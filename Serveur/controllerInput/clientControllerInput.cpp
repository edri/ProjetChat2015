/*
     * Created by Benoist Wolleb
     *
     * Implements clientControllerInput.h
*/

#include <QtCore/QDebug>
#include <QDataStream>
#include "clientControllerInput.h"
#include <cstring>

ClientControllerInput::ClientControllerInput()
{
    _controllerChat = nullptr;
    _controllerUser = nullptr;
    _controllerRoom = nullptr;
}

ClientControllerInput::ClientControllerInput(ControllerChat* controllerChat, ControllerUser* controllerUser, ControllerRoom* controllerRoom) : _controllerChat(controllerChat), _controllerUser(controllerUser), _controllerRoom(controllerRoom) {}


void ClientControllerInput::controllerChat(ControllerChat* controllerChat)
{
    _controllerChat = controllerChat;
}
void ClientControllerInput::controllerUser(ControllerUser* controllerUser)
{
    _controllerUser = controllerUser;
}

void ClientControllerInput::controllerRoom(ControllerRoom* controllerRoom)
{
    _controllerRoom = controllerRoom;
}

void ClientControllerInput::receiveMessage(ModelMessage& message, const bool edited, QObject* sender)
{
    // Sender is unused in the client controller.
    Q_UNUSED(sender);
    qDebug() << "Message recu: " << message.getContent() << " dans chambre " << message.getIdRoom() << " de user " << message.getIdUser();

    _controllerChat->receiveMessage(message, edited);
}

void ClientControllerInput::deleteMessage(const quint32 roomId, const quint32 messageId, QObject* sender)
{
    Q_UNUSED(sender);

    _controllerChat->deleteMessageInModel(roomId, messageId);
}

void ClientControllerInput::infoUser(ModelUser& user, QByteArray& keySalt, QByteArray& publicKey, QByteArray& privateKey, QObject* sender)
{
    // Sender is unused in the client controller.
    Q_UNUSED(sender);
        
    //ControllerUser
    Salt salt;
    RSAPair rsaKeys;
    
    salt.resize(keySalt.size());
    memcpy(salt.data(), keySalt.data(), salt.size());
    
    rsaKeys.privateKey.resize(privateKey.size());
    rsaKeys.publicKey.resize(publicKey.size());
    memcpy(rsaKeys.privateKey.data(), privateKey.data(), rsaKeys.privateKey.size());
    memcpy(rsaKeys.publicKey.data(), publicKey.data(), rsaKeys.publicKey.size());
    
    _controllerUser->infoUser(user, salt, rsaKeys);
}

void ClientControllerInput::join(const QMap<quint32, ModelRoom>& rooms, const QMap<quint32, ModelUser>& users, QObject* sender)
{
    Q_UNUSED(sender);
    qDebug() << "Dans Join";

    qDebug() << "Ajout des utilisateurs";
    for (ModelUser user : users)
    {
        _controllerChat->loadUser(user);
    }
    
    qDebug() << "Ajout des salles";
    for (ModelRoom room : rooms)
    {
        _controllerChat->loadRoom(room);

        /*qDebug() << "Il faut ajouter la salle " << room.getIdRoom() << ": " << room.getName();
        QSet<quint32> rusers = room.getUsers();
        QSet<quint32> radmins = room.getAdmins();
        
        for (quint32 u : rusers)
        {
            ModelUser mu = users[u];
            qDebug() << "Contenant l'utilisateur " << mu.getIdUser() << ": " << mu.getUserName();
        }
        
        for (quint32 u : radmins)
        {
            ModelUser mu = users[u];
            qDebug() << "Administré par " << mu.getIdUser() << ": " << mu.getUserName();
        }*/
    }
    qDebug() << "Chargement des salles";
    _controllerChat->loadUserRooms();
    qDebug() << "Sort Join";
}

void ClientControllerInput::userId(const QString& userName, bool exists, quint32 userId, QObject* sender)
{
    Q_UNUSED(sender);
    Q_UNUSED(userName);
    // if (controllerUser->userIsConnected()) // Need to add the method to the controller user.
    
    _controllerRoom->userId(exists, userId);
    
    // else { call controllerUser->foo();}
}

void ClientControllerInput::connected(const quint32 userId, QObject *sender)
{
    Q_UNUSED(sender);

    _controllerChat->userStatusChanged(userId, true);
}

void ClientControllerInput::disconnect(const quint32 userId, QObject* sender)
{
    Q_UNUSED(sender);

    _controllerChat->userStatusChanged(userId, false);
}

void ClientControllerInput::deleteRoom(const quint32 roomId, QObject* sender)
{
    Q_UNUSED(sender);

    _controllerChat->deleteRoomInModel(roomId);
}

void ClientControllerInput::leaveRoom(const quint32 userId, const quint32 roomId, QObject *sender)
{
    Q_UNUSED(sender);

    _controllerChat->leaveRoomInModel(userId, roomId);
}

void ClientControllerInput::salt(const QString& pseudo, const QByteArray& salt, QObject* sender)
{
    Q_UNUSED(pseudo);
    Q_UNUSED(sender);
    
    Salt s;
    s.resize(salt.size());
    memcpy(s.data(), salt.data(), salt.size());
    
    _controllerUser->receiveSalt(s);
}

void ClientControllerInput::publicKey(QList<QPair<quint32, QByteArray>>& usersIdAndKey, QObject* sender)
{
    Q_UNUSED(sender);
    _controllerRoom->createRoom(usersIdAndKey);
}

void ClientControllerInput::listRooms(const QList<QPair<quint32, QString>>& publicRooms, const QList<QPair<quint32, QString>>& privateVisibleRooms, QObject* sender)
{
    Q_UNUSED(sender);
    _controllerRoom->listRooms(publicRooms, privateVisibleRooms);
}

void ClientControllerInput::room(ModelRoom& room, bool edited, const QMap<quint32, QByteArray>& usersAndKeys, QObject* sender)
{
    Q_UNUSED(usersAndKeys);
    Q_UNUSED(sender);

    if (edited)
        _controllerChat->editRoom(room);
}

void ClientControllerInput::editAccount(ModelUser& user, const QByteArray& password, const QByteArray& privateKey, QObject* sender)
{Q_UNUSED(user); Q_UNUSED(password); Q_UNUSED(privateKey); Q_UNUSED(sender);}

void ClientControllerInput::request(const quint32 roomId, const ModelUser& user, const QByteArray& publicKey, const bool accepted, QObject* sender)
{
    Q_UNUSED(accepted);
    Q_UNUSED(sender);

    _controllerChat->newMembershipRequest(roomId, user, publicKey);
}

void ClientControllerInput::error(const ModelError& error)
{
    switch(error.getErrorType())
    {
        case ErrorType::AUTH_ERROR:
        {
            _controllerUser->authError();
        }
        break;
    }
}
