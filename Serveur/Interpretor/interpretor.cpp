#include "interpretor.h"
#include <QByteArray>
#include <QDataStream>

Interpretor::Interpretor(ControllerInput& dispatcher) : _dispatcher(dispatcher){};

QByteArray Interpretor::sendMessage(const ModelMessage& message)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    
    stream << (quint32) MessageType::MESSAGE << message;
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
    
    stream << (quint32) MessageType::INFO_USER << user;
    return data;
}

QByteArray Interpretor::sendError(const QString& text)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    
    stream << (quint32) MessageType::SERVER_ERROR << text;
    return data;
}

QByteArray Interpretor::join(const quint32 idUser, const quint32 idRoom)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    
    stream << (quint32) MessageType::JOIN << idUser << idRoom;
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
        }
        break;
        
        case MessageType::INFO_USER:
        {
            ModelUser user;
            stream >> user;
            // Il y aura aussi les clés à gérer ici (récupération de la masterkey chiffrée)
            // Envoyer cet objet quelque part
        }
        break;
        
        case MessageType::MESSAGE:
        {
            ModelMessage message;
            stream >> message;
            _dispatcher.receiveMessage(message);
        }
        break;
        
        case MessageType::LOGIN:
        {
            QString pseudo;
            QString hashedPwd;
            stream >> pseudo >> hashedPwd;
            _dispatcher.login(pseudo, hashedPwd);
        }
        break;
        
        case MessageType::JOIN:
        {
            quint32 idUser;
            quint32 idRoom;
            stream >> idUser >> idRoom;
            // Envoyer ces objets quelque part
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
            // Envoyer ces objets quelque part
        }
        break;
        
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
