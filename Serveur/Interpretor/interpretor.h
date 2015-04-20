#ifndef INTERPRETOR_H
#define INTERPRETOR_H

#include "modelChator.h"
#include "packet.h"
//#include <QObject>
#include <QByteArray>
#include <array>
#include "controllerInput.h"
#include "connector.h"

class AbstractOutput
{
    public:
    virtual QByteArray sendMessage(const ModelMessage& message) = 0;
    virtual QByteArray login(const QString& pseudo, const QString& hashedPwd) = 0;
    virtual QByteArray createAccount(const ModelUser& user) = 0;
    virtual QByteArray sendInfoUser(const ModelUser& user) = 0;
    virtual QByteArray sendError(const QString& text) = 0;
    virtual QByteArray join(const quint32 idUser, const quint32 idRoom) = 0;
    virtual QByteArray leave(const quint32 idUser, const quint32 idRoom) = 0;
    virtual QByteArray disconnect(const quint32 idUser) = 0;
};

//class Interpretor : public QObject        // Not sure yet if the Interpretor has to be a subclass of QObject (will it have signals/slots?)
class Interpretor : public AbstractOutput
{
    public:
    Interpretor(ControllerInput* dispatcher);
    
    QByteArray sendMessage(const ModelMessage& message);
    QByteArray login(const QString& pseudo, const QString& hashedPwd);
    QByteArray createAccount(const ModelUser& user);
    QByteArray sendInfoUser(const ModelUser& user);
    QByteArray sendError(const QString& text);
    QByteArray join(const quint32 idUser, const quint32 idRoom);
    QByteArray leave(const quint32 idUser, const quint32 idRoom);
    QByteArray disconnect(const quint32 idUser);
    
    public slots:
    void processData(const QByteArray& data);
    
    private:
    // Pointeur sur l'objet réseau
    ControllerInput* _dispatcher;
};

#endif
