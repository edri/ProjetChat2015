#include "interpretor.h"
#include "packet.h"
#include <QByteArray>
#include <QDataStream>

void Interpretor::sendMessage(const ModelMessage& message)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    
    stream << (quint32) MessageType::MESSAGE << message;
    // Envoyer les données quelque part
}

void Interpretor::login(const QString& pseudo, const QString& pass)
{
    // Le mot de passe est déjà hâché????????????????????
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    
    stream << (quint32) MessageType::LOGIN << pseudo << pass;
    // Envoyer les données quelque part
}

void Interpretor::createAccount(ModelUser& user)
{
    // Il y aura aussi les clés à gérer ici (envoi des deux clés asymétriques chiffrées et de la masterkey chiffrée)
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    
    stream << (quint32) MessageType::NEW_ACCOUNT << user;
    // Envoyer les données quelque part
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
            // Il y aura aussi les clés à gérer ici (récupération des deux clés asymétriques chiffrées et de la masterkey chiffrée)
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
            // Envoyer cet objet quelque part
        }
        break;
        
        case MessageType::LOGIN:
        {
            QString pseudo;
            QString hashedPwd;
            stream >> pseudo >> hashedPwd;
            // Envoyer ces objets quelque part
        }
        break;
        
        case MessageType::ERROR:
        {
            Error error;
            stream >> error;
            // Envoyer cet objet quelque part
        }
        break;
        
        default:
        break;
    }
}
