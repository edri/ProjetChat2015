#include "interpretor.h"
#include "packet.h"
#include <QByteArray>
#include <QDataStream>

void Interpretor::sendMessage(const ModelMessage& message)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    
    stream << (quint32) MessageType::SENDMESSAGE << message;
}
