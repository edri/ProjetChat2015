#ifndef SERVER_CONTROLLER_INPUT_H
#define SERVER_CONTROLLER_INPUT_H

#include "../controllerInput/controllerInput.h"
#include "controllerUser.h"
#include "controllerRoom.h"
#include "serverModel.h"

class ServerControllerInput : public ControllerInput
{
    Q_OBJECT
    
    public:
    ServerControllerInput(ControllerUser& controllerUser, ControllerRoom& controllerRoom);
    
    void createAccount(ModelUser& user, QObject* sender);
    void editAccount(ModelUser& user, QObject* sender);
    void receiveMessage(ModelMessage& message, const bool edited, QObject* sender);
    void deleteMessage(const quint32 messageId, QObject* sender);
    void login(const QString& pseudo, const QString& hashedPWD, QObject* sender);
    void infoUser(ModelUser& user, QObject* sender);
    void room(ModelRoom& room, bool edited, QObject* sender);
    void userId(const QString& userName, bool exists, quint32 userId, QObject* sender);
    void disconnect(const quint32 userId, QObject* sender);
    void deleteRoom(const quint32 roomId, QObject* sender);
    void leaveRoom(const quint32 userId, const quint32 roomId, QObject* sender);
    
    private:
    ControllerUser& _controllerUser;
    ControllerRoom& _controllerRoom;
};

#endif
