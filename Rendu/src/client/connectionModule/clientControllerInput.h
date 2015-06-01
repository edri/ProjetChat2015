/*
     * Contains methods called everytime a user receive instruction from
     * the server. This is a concrete subclass that implements the methods
     * defined in its parent: ControllerInput.
*/

#ifndef CLIENT_CONTROLLER_INPUT_H
#define CLIENT_CONTROLLER_INPUT_H

#include "../../common/controllerInput/controllerInput.h"
#include "../chatModule/controllerChat.h"
#include "../roomModule/controllerRoom.h"
#include "../userModule/controllerUser.h"

// We have to predeclare the controllers so that we can use them
class ControllerUser;
class ControllerChat;

class ClientControllerInput : public ControllerInput
{
    ControllerChat* _controllerChat;
    ControllerUser* _controllerUser;
    ControllerRoom* _controllerRoom;
    
    public:
    
    // Constructors
    ClientControllerInput();
    ClientControllerInput(ControllerChat* controllerChat, ControllerUser* controllerUser, ControllerRoom* controllerRoom);
    
    // Setters
    void controllerChat(ControllerChat* controllerChat);
    void controllerUser(ControllerUser* controlleruser);
    void controllerRoom(ControllerRoom* controllerRoom);
    
    // Concrete methods that implement the ControllerInput interface. See controllerInput.h for more details.
    void receiveMessage(ModelMessage& message, const bool edited, QObject* sender);
    void deleteMessage(const quint32 roomId, const quint32 messageId, QObject* sender);
    void infoUser(ModelUser& user, QByteArray& keySalt, QByteArray& publicKey, QByteArray& privateKey, QObject* sender);
    void join(const QMap<quint32, ModelRoom>& rooms, const QMap<quint32, ModelUser>& users, QObject* sender);
    void userId(const QString& userName, bool exists, quint32 userId, QObject* sender);
    void connected(const quint32 userId, QObject* sender);
    void disconnect(const quint32 userId, QObject* sender);
    void deleteRoom(const quint32 roomId, QObject* sender);
    void leaveRoom(const quint32 userId, const quint32 roomId, QObject* sender);
    void salt(const QString& pseudo, const QByteArray& salt, QObject* sender);
    void publicKey(QList<QPair<quint32, QByteArray>>& usersIdAndKey, QObject* sender);
    void listRooms(const QList<QPair<quint32, QString>>& publicRooms, const QList<QPair<quint32, QString>>& privateVisibleRooms, QObject* sender);
    void room(ModelRoom& room, bool edited, const QMap<quint32, QByteArray>& usersAndKeys, QObject* sender);
    void editAccount(ModelUser& user, const QByteArray& password, const QByteArray& privateKey, QObject* sender);
    void request(const quint32 roomId, const ModelUser& user, const QByteArray& publicKey, const bool accepted, QObject* sender);
    void error(const ModelError& error);
};

#endif // CLIENT_CONTROLLER_INPUT_H
