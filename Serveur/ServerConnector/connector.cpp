#include "connector.h"
#include <QUrl>

Connector::Connector() : isConnected(false)
{
    connect(&_socket, SIGNAL(connected()), this, SLOT(connected()));
}

void Connector::connectToServer(QString url)
{
    _socket.open(QUrl("wss://" + url));
}

void Connector::connected()
{
    
}

void Connector::send(const QByteArray& data)
{
    if (isConnected)
    {
        _socket.sendBinaryMessage(data);
    }
}
