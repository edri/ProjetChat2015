#include "interpretor.h"
#include <QByteArray>
#include <QDataStream>

Interpretor::Interpretor(ControllerInput& dispatcher) : _dispatcher(dispatcher){};

QByteArray Interpretor::sendMessage(const ModelMessage& message, const bool edited)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    
    stream << (quint32) MessageType::MESSAGE << edited << message;
    return data;
}

QByteArray Interpretor::login(const QString& pseudo, const QString& hashedPwd)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    
    stream << (quint32) MessageType::LOGIN << pseudo << hashedPwd;
    return data;
}

QByteArray Interpretor::createAccount(const ModelUser& user)
{
    // Il y aura aussi les clés à gérer ici (envoi des deux clés asymétriques et de la masterkey chiffrée)
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    
    stream << (quint32) MessageType::NEW_ACCOUNT << user;
    return data;
}

QByteArray Interpretor::sendInfoUser(const ModelUser& user)
{
    // Il y aura aussi les clés à gérer ici (envoi de la clé publique et éventuellement de la masterkey chiffrée)
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    qDebug() << "serialisation de " << user.getUserName();
    stream << (quint32) MessageType::INFO_USER << user;
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

QByteArray Interpretor::room(const ModelRoom& room, bool edited)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    
    stream << (quint32) MessageType::ROOM << edited << room;
    return data;
}

QByteArray Interpretor::userId(const QString& userName, bool exists, quint32 userId)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    
    stream << (quint32) MessageType::USER_ID << userName << exists << userId;
    return data;
}

void Interpretor::processData(const QByteArray& data)
{
    QDataStream stream(data);
    quint32 type;
    stream >> type;
    
    switch ((MessageType) type)
    {
        case MessageType::NEW_ACCOUNT:
        {
            ModelUser user;
            stream >> user;
            // Il y aura aussi les clés à gérer ici (récupération des deux clés asymétriques et de la masterkey chiffrée)
            // Envoyer cet objet quelque part
            _dispatcher.createAccount(user, sender());
        }
        break;
        
        case MessageType::INFO_USER:
        {
            ModelUser user;
            stream >> user;
            qDebug() << "Déserialisation info user";
            // Il y aura aussi les clés à gérer ici (récupération de la masterkey chiffrée)
            _dispatcher.infoUser(user, sender());
        }
        break;
        
        case MessageType::MESSAGE:
        {
            bool edited;
            ModelMessage message;
            stream >> edited;
            stream >> message;
            _dispatcher.receiveMessage(message, edited, sender());
        }
        break;
        
        case MessageType::LOGIN:
        {
            QString pseudo;
            QString hashedPwd;
            stream >> pseudo >> hashedPwd;
            _dispatcher.login(pseudo, hashedPwd, sender());
        }
        break;
        
        case MessageType::USER_JOIN:
        {
            quint32 idUser;
            quint32 idRoom;
            stream >> idUser >> idRoom;
            // Envoyer ces objets quelque part
        }
        break;
        
        case MessageType::JOIN:
        {
            qDebug() << "Déserialisation join";
            QMap<quint32, ModelRoom> rooms;
            QMap<quint32, ModelUser> users;
            stream >> rooms >> users;
            _dispatcher.join(rooms, users, sender());
        }
        break;
        
        case MessageType::LEAVE:
        {
            quint32 idUser;
            quint32 idRoom;
            stream >> idUser >> idRoom;
            // Envoyer ces objets quelque part
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
            // Envoyer ces objets quelque part
        }
        break;
        
        case MessageType::ROOM:
        {
            bool edited;
            ModelRoom room;
            stream >> edited;
            stream >> room;
            _dispatcher.room(room, edited, sender());
        }
        
        case MessageType::USER_ID:
        {
            QString userName;
            bool exists;
            quint32 userId;
            stream >> userName;
            stream >> exists;
            stream >> userId;
            _dispatcher.userId(userName, exists, userId, sender());
        }
        
        case MessageType::SERVER_ERROR:
        {
            ModelError error;
            stream >> error;
            // Envoyer cet objet quelque part
        }
        break;
        
        default:
        break;
    }
}
