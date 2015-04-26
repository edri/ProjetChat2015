#include <QtCore/QDebug>
#include "controllerInput.h"
#include "../../ModeleChator/modelChator.h"

void ClientControllerInput::receiveMessage(ModelMessage& message, QObject* sender)
{
    Q_UNUSED(sender);
    
}

void ClientControllerInput::infoUser(ModelUser& user, QObject* sender)
{
    qDebug() << "Info recue: " << user.getIdUser() << ", " << user.getUserName();
}
