#include "listener.h"
#include <QFile>
#include <QWebSocket>
#include <QSslKey>

Listener::Listener(quint16 port) : _server("Chator", QWebSocketServer::SecureMode, this)
{
    QSslConfiguration sslConfiguration;
    QFile certFile(QStringLiteral("cert.pem"));
    QFile keyFile(QStringLiteral("key.pem"));
    certFile.open(QIODevice::ReadOnly);
    keyFile.open(QIODevice::ReadOnly);
    QSslCertificate certificate(&certFile, QSsl::Pem);
    QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
    certFile.close();
    keyFile.close();
    sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConfiguration.setLocalCertificate(certificate);
    sslConfiguration.setPrivateKey(sslKey);
    sslConfiguration.setProtocol(QSsl::SecureProtocols);
    _server.setSslConfiguration(sslConfiguration);

    if (_server.listen(QHostAddress::AnyIPv4, port))
    {
        qDebug() << "SSL Echo Server listening on port" << port;
        connect(&_server, &QWebSocketServer::newConnection, this, &Listener::newConnection);
        connect(&_server, &QWebSocketServer::sslErrors, this, &Listener::SSLErrors);
    }
}

Listener::~Listener()
{
    _server.close();
    qDeleteAll(_clients.begin(), _clients.end());
}

void Listener::newConnection()
{
    QWebSocket *pSocket = _server.nextPendingConnection();

    qDebug() << "Client connected:" << pSocket->peerName() << pSocket->origin();

    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &Listener::receive);
    connect(pSocket, &QWebSocket::disconnected, this, &Listener::disconnected);

    _clients << pSocket;
}

void Listener::receive(QByteArray message)
{
    Q_UNUSED(message)
    qDebug() << "New message";
}

void Listener::disconnected()
{
    qDebug() << "Client disconnected";
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient)
    {
        _clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

void Listener::SSLErrors(const QList<QSslError> &)
{
    qDebug() << "Ssl errors occurred";
}
