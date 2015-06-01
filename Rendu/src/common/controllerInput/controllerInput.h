/*
     * Abstract class which contains every methods a programm should
     * implements in order to receive informations from remote sender.
     * All the methods are virtual so that this class defines an interface
     * and the invocation will be totally transparent for the user of it.
     * Some methods have a body that does nothing so the subclasses are not
     * forced to implement them.
     * 
     * For more details for each packets that can be sent, please refer to the
     * main documentation
*/

#ifndef CONTROLLER_INPUT_H
#define CONTROLLER_INPUT_H

#include <QtCore/QObject>
#include "../models/packet.h"
#include "./models/modelChator.h"

class ControllerInput : public QObject
{
    Q_OBJECT
    
    public:
    
    // Method called when the remote client wants to create a new account.
    virtual void createAccount(ModelUser& user, const QByteArray& password, const QByteArray& passwordSalt, const QByteArray& keySalt, const QByteArray& privateKey, const QByteArray& publicKey, QObject* sender)
    {
        Q_UNUSED(user);
        Q_UNUSED(password);
        Q_UNUSED(passwordSalt);
        Q_UNUSED(keySalt);
        Q_UNUSED(privateKey);
        Q_UNUSED(publicKey);
        Q_UNUSED(sender);
    }
    
    // Method called when the remote client wants to edit his profile.
    virtual void editAccount(ModelUser& user, const QByteArray& password, const QByteArray& privateKey, QObject* sender) = 0;
    
    // Method called when the remote client sends a message.
    virtual void receiveMessage(ModelMessage& message, const bool edited, QObject* sender) = 0;
    
    // Method called when the remote client wants to delete a message.
    virtual void deleteMessage(const quint32 roomId, const quint32 messageId, QObject* sender) = 0;
    
    // Method called to tell the remote client that a user has connected.
    virtual void connected(const quint32 userId, QObject* sender)
    {
        Q_UNUSED(userId);
        Q_UNUSED(sender)
    }
    
    // Method called to tell the remote client that a user has disconnected.
    virtual void disconnect(const quint32 userId, QObject* sender)
    {
        Q_UNUSED(userId);
        Q_UNUSED(sender);
    }
    
    // Method called when the remote client wants to login.
    virtual void login(const QString& pseudo, const QByteArray& hashedPWD, QObject* sender)
    {
        Q_UNUSED(pseudo);
        Q_UNUSED(hashedPWD);
        Q_UNUSED(sender);
    }
    
    // Method called to request and get the salt of a user.
    virtual void salt(const QString& pseudo, const QByteArray& salt, QObject* sender) = 0;
    
    // Method called to request and get the public keys for a bunch of users.
    virtual void publicKey(QList<QPair<quint32, QByteArray>>& usersIdAndKey, QObject* sender) = 0;
    
    // Method called to tell the remote client his ModelUser and his keyring.
    virtual void infoUser(ModelUser& user, QByteArray& keySalt, QByteArray& publicKey, QByteArray& privateKey, QObject* sender)
    {
        Q_UNUSED(user);
        Q_UNUSED(keySalt);
        Q_UNUSED(publicKey);
        Q_UNUSED(privateKey);
        Q_UNUSED(sender);
    }
    
    // Method called to tell the new ModelRoom when the room is edited.
    virtual void room(ModelRoom& room, bool edited, const QMap<quint32, QByteArray>& usersAndKeys, QObject* sender) = 0;
    
    // Method called to tell the users when new users arrive.
    virtual void join(const QMap<quint32, ModelRoom>& rooms, const QMap<quint32, ModelUser>& users, QObject* sender)
    {
        Q_UNUSED(rooms);
        Q_UNUSED(users);
        Q_UNUSED(sender);
    }
    
    // Method called to request and get the ID of a user and check its availability.
    virtual void userId(const QString& userName, bool exists, quint32 userId, QObject* sender)
    {
        Q_UNUSED(userName);
        Q_UNUSED(exists);
        Q_UNUSED(userId);
        Q_UNUSED(sender);
    }
    
    // Method called when the remote client wants to delete a room.
    virtual void deleteRoom(const quint32 roomId, QObject* sender) = 0;
    
    // Method called when the remote client wants to leave a room.
    virtual void leaveRoom(const quint32 userId, const quint32 roomId, QObject* sender) = 0;
    
    // Method called when the remote client wants to list every available room where he could join
    virtual void listRooms(const QList<QPair<quint32, QString>>& publicRooms, const QList<QPair<quint32, QString>>& privateVisibleRooms, QObject* sender) = 0;
    
    // Method called when an administrator has to manage the requests for a private room
    virtual void request(const quint32 roomId, const ModelUser& user, const QByteArray& publicKey, const bool accepted, QObject* sender) = 0;
    
    // Method called when the remote client wants to join a room.
    virtual void joinRoom(const quint32 roomId, QObject* sender)
    {
        Q_UNUSED(roomId);
        Q_UNUSED(sender);
    }
    
    // Method called to tell the remote client an error.
    virtual void error(const ModelError& error) {Q_UNUSED(error);}
};

#endif
