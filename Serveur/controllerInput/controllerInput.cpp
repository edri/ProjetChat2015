#include <QtCore/QDebug>
#include "controllerInput.h"
#include "../../ModeleChator/modelChator.h"

void ClientControllerInput::receiveMessage(ModelMessage& message, QObject* sender)
{
    Q_UNUSED(sender);
    Q_UNUSED(message);
}

void ClientControllerInput::infoUser(ModelUser& message, QObject* sender)
{
    Q_UNUSED(message);
    Q_UNUSED(sender);
}

void infoUser(ModelUser& user, QObject* sender)
{
    Q_UNUSED(user);
    Q_UNUSED(sender);
}
