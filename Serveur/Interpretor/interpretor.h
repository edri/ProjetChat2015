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
    
    QByteArray sendMessage(const ModelMessage& message);
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
    
    public slots:
    void processData(const QByteArray& data);
    
    private:
    ControllerInput& _dispatcher;
};

#endif
