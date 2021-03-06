/*
     * Implements interpretor.h.
*/

#include "interpretor.h"
#include <QByteArray>
#include <QDataStream>

// The constructor sets the reference to the controllerInput
Interpretor::Interpretor(ControllerInput& dispatcher) : _dispatcher(dispatcher){};


// Every following method serializes the packet identifier and the parameters.

QByteArray Interpretor::sendMessage(const ModelMessage& message, const bool edited)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    
    stream << (quint32) MessageType::MESSAGE << edited << message;
    return data;
}

QByteArray Interpretor::deleteMessage(const quint32 roomId, const quint32 messageId)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    stream << (quint32) MessageType::DELETE_MESSAGE << roomId << messageId;
    return data;
}

QByteArray Interpretor::login(const QString& pseudo, const Hash& hashedPwd)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    
    stream << (quint32) MessageType::LOGIN << pseudo << hashedPwd;
    return data;
}

QByteArray Interpretor::createAccount(const ModelUser& user, const Hash& password, const Salt& passwordSalt, const Salt& keySalt, const RSAPair& asymKeys)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    
    stream << (quint32) MessageType::NEW_ACCOUNT << user << password << passwordSalt << keySalt << asymKeys;
    return data;
}

QByteArray Interpretor::editAccount(const ModelUser& user, const QByteArray& password, const QByteArray& privateKey)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    
    stream << (quint32) MessageType::EDIT_ACCOUNT << user << password << privateKey;
    return data;
}

QByteArray Interpretor::sendInfoUser(const ModelUser& user, const QByteArray& keySalt, const QByteArray& privateKey, const QByteArray& publicKey)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    stream << (quint32) MessageType::INFO_USER << user << keySalt << privateKey << publicKey;
    return data;
}

QByteArray Interpretor::sendError(const ModelError& error)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    
    stream << (quint32) MessageType::SERVER_ERROR << error;
    return data;
}

QByteArray Interpretor::userJoin(const quint32 idUser, const quint32 idRoom)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    
    stream << (quint32) MessageType::JOIN << idUser << idRoom;
    return data;
}

QByteArray Interpretor::join(const QMap<quint32, ModelRoom>& rooms, const QMap<quint32, ModelUser>& users)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    
    stream << (quint32) MessageType::JOIN << rooms << users;
    return data;
}

QByteArray Interpretor::leave(const quint32 idUser, const quint32 idRoom)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    
    stream << (quint32) MessageType::LEAVE << idUser << idRoom;
    return data;
}

QByteArray Interpretor::disconnect(const quint32 idUser)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    
    stream << (quint32) MessageType::DISCONNECT << idUser;
    return data;
}

QByteArray Interpretor::connected(const ModelUser& user)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    
    stream << (quint32) MessageType::CONNECTED << user;
    return data;
}

QByteArray Interpretor::room(const ModelRoom& room, const QMap<quint32, QByteArray>& usersAndKeys, bool edited)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    stream << (quint32) MessageType::ROOM << edited << room << usersAndKeys;
    return data;
}

QByteArray Interpretor::userId(const QString& userName, bool exists, quint32 userId)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    
    stream << (quint32) MessageType::USER_ID << userName << exists << userId;
    return data;
}

QByteArray Interpretor::deleteRoom(const quint32 roomId)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    stream << (quint32) MessageType::DELETE_ROOM << roomId;
    return data;
}

QByteArray Interpretor::joinRoom(const quint32 roomId)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    stream << (quint32) MessageType::JOIN_ROOM << roomId;
    return data;
}

QByteArray Interpretor::salt(const QString& pseudo, const QByteArray& salt)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    stream << (quint32) MessageType::SALT << pseudo << salt;
    return data;
}

QByteArray Interpretor::publicKey(const QList<QPair<quint32, QByteArray>>& usersIdAndKey)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    stream << (quint32) MessageType::PUBLIC_KEY << usersIdAndKey;
    return data;
}

QByteArray Interpretor::request(const quint32 roomId, const ModelUser& user, const QByteArray& publicKey, bool accepted)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    stream << (quint32) MessageType::REQUEST << roomId << user << publicKey << accepted;
    return data;
}

QByteArray Interpretor::listRooms(const QList<QPair<quint32, QString>>& publicRooms, const QList<QPair<quint32, QString>>& privateVisibleRooms)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    stream << (quint32) MessageType::LIST_ROOMS << publicRooms << privateVisibleRooms;
    return data;
}

void Interpretor::processData(const QByteArray& data)
{
    QDataStream stream(data);
    
    // We first deserializes the packet identifier and pass it to the switch
    quint32 type;
    stream >> type;
    
    // Each case of this switch corresponds to a different packet identifier. Each one will deserialize the parameters and transmit everything to the controllerInput.
    // We had to use braces in order to create variables inside a case of a switch.
    switch ((MessageType) type)
    {
        case MessageType::NEW_ACCOUNT:
        {
            ModelUser user;
            QByteArray password;
            QByteArray passwordSalt;
            QByteArray keySalt;
            QByteArray privateKey;
            QByteArray publicKey;
            
            stream >> user >> password >> passwordSalt >> keySalt >> privateKey >> publicKey;
            _dispatcher.createAccount(user, password, passwordSalt, keySalt, privateKey, publicKey, sender());
        }
        break;
        
        case MessageType::EDIT_ACCOUNT:
        {
            ModelUser user;
            QByteArray password;
            QByteArray privateKey;
            
            stream >> user >> password >> privateKey;
            _dispatcher.editAccount(user, password, privateKey, sender());
        }
        break;
        
        case MessageType::INFO_USER:
        {
            ModelUser user;
            QByteArray keySalt;
            QByteArray privateKey;
            QByteArray publicKey;
            
            stream >> user >> keySalt >> privateKey >> publicKey;
            _dispatcher.infoUser(user, keySalt, publicKey, privateKey, sender());
        }
        break;
        
        case MessageType::MESSAGE:
        {
            bool edited;
            ModelMessage message;
            
            stream >> edited >> message;
            _dispatcher.receiveMessage(message, edited, sender());
        }
        break;
        
        case MessageType::DELETE_MESSAGE:
        {
            quint32 roomId;
            quint32 messageId;
            
            stream >> roomId >> messageId;
            _dispatcher.deleteMessage(roomId, messageId, sender());
        }
        break;

        case MessageType::LOGIN:
        {
            QString pseudo;
            QByteArray hashedPwd;
            
            stream >> pseudo >> hashedPwd;
            _dispatcher.login(pseudo, hashedPwd, sender());
        }
        break;
        
        case MessageType::JOIN:
        {
            QMap<quint32, ModelRoom> rooms;
            QMap<quint32, ModelUser> users;
            
            stream >> rooms >> users;
            _dispatcher.join(rooms, users, sender());
        }
        break;
        
        case MessageType::LEAVE:
        {
            quint32 roomId;
            quint32 userId;
            
            stream >> userId >> roomId;
            _dispatcher.leaveRoom(userId, roomId, sender());
        }
        break;
        
        case MessageType::DISCONNECT:
        {
            quint32 idUser;
            
            stream >> idUser;
            _dispatcher.disconnect(idUser, sender());
        }
        break;
        
        case MessageType::CONNECTED:
        {
            ModelUser user;
            
            stream >> user;
            _dispatcher.connected(user.getIdUser(), sender());
        }
        break;
        
        case MessageType::ROOM:
        {
            bool edited;
            ModelRoom room;
            QMap<quint32, QByteArray> usersAndKeys;
            stream >> edited;
            stream >> room;
            stream >> usersAndKeys;
            _dispatcher.room(room, edited, usersAndKeys, sender());
        }
        break;

        case MessageType::USER_ID:
        {
            QString userName;
            bool exists;
            quint32 userId;
            
            stream >> userName >> exists >> userId;
            _dispatcher.userId(userName, exists, userId, sender());
        }
        break;

        case MessageType::DELETE_ROOM:
        {
            quint32 roomId;
            
            stream >> roomId;
            _dispatcher.deleteRoom(roomId, sender());
        }
        break;
        
        case MessageType::SALT:
        {
            QString pseudo;
            QByteArray salt;
            
            stream >> pseudo >> salt;
            _dispatcher.salt(pseudo, salt, sender());
        }
        break;
        
        case MessageType::REQUEST:
        {
            quint32 idRoom;
            ModelUser user;
            QByteArray key;
            bool accepted;
            
            stream >> idRoom >> user >> key >> accepted;
            _dispatcher.request(idRoom, user, key, accepted, sender());
        }
        break;
        
        case MessageType::PUBLIC_KEY:
        {
            QList<QPair<quint32, QByteArray>> idsAndKeys;
            
            stream >> idsAndKeys;
            _dispatcher.publicKey(idsAndKeys, sender());
        }
        break;
        
        case MessageType::LIST_ROOMS:
        {
            QList<QPair<quint32, QString>> publicRooms;
            QList<QPair<quint32, QString>> privateVisibleRooms;
            
            stream >> publicRooms >> privateVisibleRooms;
            _dispatcher.listRooms(publicRooms, privateVisibleRooms, sender());
        }
        break;
        
        case MessageType::JOIN_ROOM:
        {
            quint32 roomId;
            
            stream >> roomId;
            _dispatcher.joinRoom(roomId, sender());
        }
        break;
        
        case MessageType::SERVER_ERROR:
        {
            ModelError error;
            
            stream >> error;
            _dispatcher.error(error);
        }
        break;
        
        default:
        break;
    }
}
