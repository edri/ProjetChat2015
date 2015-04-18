#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QtCore/QObject>
#include "modelChator.h"

class ControllerInput : public QObject
{
    Q_OBJECT
    
    public:
    virtual void receiveMessage(const ModelMessage& message) = 0;
};

class ClientControllerInput : public ControllerInput
{
    Q_OBJECT
    
    public:
    virtual void receiveMessage(const ModelMessage& message);
};

#endif
