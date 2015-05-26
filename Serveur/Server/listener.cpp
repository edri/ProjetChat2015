/*
     * Created by Benoist Wolleb
     *
     * Listen for new clients and allow connection and disconnection.
*/

#include "listener.h"
#include <QFile>
#include <QWebSocket>
#include <QSslKey>
#include "../controllerInput/controllerInput.h"
#include "../Interpretor/interpretor.h"
#include "serverModel.h"

Listener::Listener(quint16 port, Interpretor& interpretor, ServerControllerInput& controllerInput) : _server("Chator", QWebSocketServer::SecureMode, this), _interpretor(interpretor), _controllerInput(controllerInput)
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
        qDebug() << "Chator listening on port" << port;
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
    QWebSocket *newSocket = _server.nextPendingConnection();
    ChatorClient* client = new ChatorClient(*newSocket);
    _clients << client;

    qDebug() << "Client connected:" << newSocket->peerAddress().toString();
    
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(client, SIGNAL(binaryMessageReceived(const QByteArray&)), &_interpretor, SLOT(processData(const QByteArray&)));
}

void Listener::disconnected()
{
    qDebug() << "Client disconnected";
    
    ChatorClient* client = (ChatorClient*) sender();
    
    if (client)
    {
        _clients.removeAll(client);
    }
    
    _controllerInput.disconnect(client->id, (QObject*) client);
}

void Listener::SSLErrors(const QList<QSslError> &)
{
    qDebug() << "Ssl errors occurred";
}
