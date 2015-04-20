#include "connector.h"
#include <QUrl>

ClientConnector::ClientConnector() : _isConnected(false)
{
    connect(&_socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(&_socket, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(sslErrors(const QList<QSslError>&)));
}

void ClientConnector::connectToServer(QString url)
{
    _socket.open(QUrl("wss://" + url));
}

void ClientConnector::send(const QByteArray& data)
{
    if (_isConnected)
    {
        _socket.sendBinaryMessage(data);
    }
}

void ClientConnector::connected()
{
    _isConnected = true;
}

void ClientConnector::sslErrors(const QList<QSslError>& errors)
{
    _socket.ignoreSslErrors();
}
