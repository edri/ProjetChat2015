#include "connector.h"
#include <QUrl>

Connector::Connector() : isConnected(false)
{
    connect(&_socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(&_socket, SIGNAL(sslErrors(const QList<QSslError>& errors)), this, SLOT(sslError(const QList<QSslError>& errors)));
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

void Connector::sslError(const QList<QSslError>& errors)
{
    
}
