#include <QtCore/QDebug>
#include "controllerInput.h"
#include "../../ModeleChator/modelChator.h"

void ClientControllerInput::receiveMessage(const ModelMessage& message, QObject* sender)
{
    Q_UNUSED(sender);
    Q_UNUSED(message);
}

void ClientControllerInput::login(const QString& pseudo, const QString& hashedPWD, QObject* sender)
{
    Q_UNUSED(pseudo);
    Q_UNUSED(hashedPWD); 
    Q_UNUSED(sender);
}
