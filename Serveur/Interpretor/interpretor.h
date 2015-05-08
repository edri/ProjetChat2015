#ifndef INTERPRETOR_H
#define INTERPRETOR_H

#include "../../ModeleChator/modelChator.h"
#include "packet.h"
//#include <QObject>
#include <QByteArray>
#include "../controllerInput/controllerInput.h"
#include "../ServerConnector/connector.h"

//class Interpretor : public QObject        // Not sure yet if the Interpretor has to be a subclass of QObject (will it have signals/slots?)
class Interpretor : public QObject
{
    Q_OBJECT
    
    public:
    Interpretor(ControllerInput& dispatcher);

    // Serialize a message and the flag indicating if the message was edited or created.
    // Last edited by Miguel Santamaria on 08.05.2015 07:21.
    QByteArray sendMessage(const ModelMessage& message, const bool edited = false);
    QByteArray login(const QString& pseudo, const QString& hashedPwd);
    QByteArray createAccount(const ModelUser& user);
    QByteArray sendInfoUser(const ModelUser& user);
    QByteArray sendError(const ModelError& error);
    QByteArray userJoin(const quint32 idUser, const quint32 idRoom);
    QByteArray join(const QMap<quint32, ModelRoom>& rooms, const QMap<quint32, ModelUser>& users);
    QByteArray leave(const quint32 idUser, const quint32 idRoom);
    QByteArray disconnect(const quint32 idUser);
    QByteArray connected(const ModelUser& user);
    
    // Serialize a room and the flage indicating if the room was edited or created.
    // The message type will be ROOM.
    // The flag is written first, the room second.
    // Created Jan Purro 26.04.2015 14:55
    QByteArray room(const ModelRoom& room, bool edited);
    
    // Serialize a user name (packet is used to ask the server if a username is used and obtain the user's id if he exists).
    // userId and exists are not used by the client, the server use them to answer the clien request.
    // The message type will be USER_ID. The parameters are written in the same order they appear in the prototype.
    // Created Jan Purro 27.04.2015 13:35
    QByteArray userId(const QString& userName, bool exists, quint32 userId);
    
    public slots:
    void processData(const QByteArray& data);
    
    private:
    ControllerInput& _dispatcher;
};

#endif
