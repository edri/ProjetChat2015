#include <QtCore/QDebug>
#include "controllerInput.h"
#include "../../ModeleChator/modelChator.h"

void ClientControllerInput::receiveMessage(ModelMessage& message, QObject* sender)
{
    Q_UNUSED(sender);
    Q_UNUSED(message);
}
