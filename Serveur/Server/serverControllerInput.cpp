#include "serverControllerInput.h"

void ServerControllerInput::receiveMessage(const ModelMessage& message)
{
    Q_UNUSED(message);
}

void ServerControllerInput::login(const QString& pseudo, const QString& hashedPWD)
{
    Q_UNUSED(pseudo);
    Q_UNUSED(hashedPWD);
    qDebug() << "login from: " << pseudo << hashedPWD;
}
