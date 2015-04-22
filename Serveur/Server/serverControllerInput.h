#ifndef SERVER_CONTROLLER_INPUT_H
#define SERVER_CONTROLLER_INPUT_H

#include "../controllerInput/controllerInput.h"

class ServerControllerInput : public ControllerInput
{
    Q_OBJECT
    
    public:
    
    
    virtual void receiveMessage(const ModelMessage& message);
    virtual void login(const QString& pseudo, const QString& hashedPWD);
};

#endif
