#ifndef CONTROLLER_INPUT_H
#define CONTROLLER_INPUT_H

#include <QtCore/QObject>
#include "../../ModeleChator/modelChator.h"

class ControllerInput : public QObject
{
    Q_OBJECT
    
    public:
    virtual void receiveMessage(const ModelMessage& message, QObject* sender) = 0;
    virtual void login(const QString& pseudo, const QString& hashedPWD, QObject* sender) {Q_UNUSED(pseudo); Q_UNUSED(hashedPWD); Q_UNUSED(sender);
}
};

class ClientControllerInput : public ControllerInput
{
    Q_OBJECT
    
    public:
    virtual void receiveMessage(const ModelMessage& message, QObject* sender);
};

#endif
