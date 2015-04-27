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

void ClientControllerInput::receiveMessage(ModelMessage& message, QObject* sender)
{
    // Sender is unused in the client controller.
    Q_UNUSED(sender);

    _controllerChat->receiveMessage(message);
}

void ClientControllerInput::infoUser(ModelUser& user, QObject* sender)
{
    // Sender is unused in the client controller.
    Q_UNUSED(sender);
    
    qDebug() << "Info recue: " << user.getIdUser() << ", " << user.getUserName();
    
    //ControllerUser Missing method
    //_controllerUser.infoUser(user);
}

void ClientControllerInput::room(const ModelRoom& room, bool edited, QObject* sender)
{
    // Sender is unused in the client controller.
    Q_UNUSED(sender);
    _controllerRoom->roomConfirmation(room, edited);
}

void ClientControllerInput::join(const QMap<quint32, ModelRoom>& rooms, const QMap<quint32, ModelUser>& users, QObject* sender)
{
    qDebug() << "dans join";
    for (ModelRoom mr : rooms)
    {
        qDebug() << "Il faut ajouter la salle " << mr.getIdRoom() << ": " << mr.getName();
        QSet<quint32> rusers = mr.getUsers();
        QSet<quint32> radmins = mr.getAdmins();
        
        for (quint32 u : rusers)
        {
            ModelUser mu = users[u];
            qDebug() << "Contenant l'utilisateur " << mu.getIdUser() << ": " << mu.getUserName();
        }
        
        for (quint32 u : radmins)
        {
            ModelUser mu = users[u];
            qDebug() << "Administré par " << mu.getIdUser() << ": " << mu.getUserName();
        }
    }
    Q_UNUSED(rooms); Q_UNUSED(users); Q_UNUSED(sender);
}
