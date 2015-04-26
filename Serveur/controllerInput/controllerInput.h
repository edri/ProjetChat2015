#ifndef CONTROLLER_INPUT_H
#define CONTROLLER_INPUT_H

#include <QtCore/QObject>
#include "../../ModeleChator/modelChator.h"

class ControllerInput : public QObject
{
    Q_OBJECT
    
    public:
    virtual void receiveMessage(ModelMessage& message, QObject* sender) = 0;
    virtual void login(const QString& pseudo, const QString& hashedPWD, QObject* sender) {Q_UNUSED(pseudo); Q_UNUSED(hashedPWD); Q_UNUSED(sender);}
    virtual void infoUser(ModelUser& user, QObject* sender) = 0;
    virtual void join(const QMap<quint32, ModelRoom>& rooms, const QMap<quint32, ModelUser>& users, QObject* sender) {Q_UNUSED(rooms); Q_UNUSED(users); Q_UNUSED(sender);}
};

#endif
