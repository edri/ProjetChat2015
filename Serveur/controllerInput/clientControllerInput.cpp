#include <QtCore/QDebug>
#include "clientControllerInput.h"

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

void ClientControllerInput::infoUser(ModelUser& user, QObject* sender)
{
    // Sender is unused in the client controller.
    Q_UNUSED(sender);
    
    qDebug() << "Info recue: " << user.getIdUser() << ", " << user.getUserName();
    
    //ControllerUser
    _controllerUser->infoUser(user);
}

void ClientControllerInput::join(const QMap<quint32, ModelRoom>& rooms, const QMap<quint32, ModelUser>& users, QObject* sender)
{
    qDebug() << "dans join";

    for (ModelUser user : users)
    {
        _controllerChat->loadUser(user);
    }

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

    _controllerChat->loadUserRooms();

    Q_UNUSED(rooms); Q_UNUSED(users); Q_UNUSED(sender);
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
    Q_UNUSED(userId);
    Q_UNUSED(sender);

    _controllerChat->leaveRoomInModel(roomId);
}
