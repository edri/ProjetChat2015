/*
     * Listen for new clients and allow connection and disconnection.
*/

#include "listener.h"
#include <QFile>
#include <QWebSocket>
#include <QSslKey>
#include "../../common/controllerInput/controllerInput.h"
#include "../../common/interpretor/interpretor.h"
#include "../../common/models/chatorConstants.h"
#include "serverModel.h"

Listener::Listener(quint16 port, Interpretor& interpretor, ServerControllerInput& controllerInput) : _server("Chator", QWebSocketServer::SecureMode, this), _interpretor(interpretor), _controllerInput(controllerInput)
{
    // Try to open the certificate and the key file
    QSslConfiguration sslConfiguration;
    QFile certFile(SERVER_CERTIFICATE_FILE);
    QFile keyFile(SERVER_KEY_FILE);    
    
    // If we cannot find or read them, it is a critical error, we can't continue
    if (!certFile.open(QIODevice::ReadOnly) || !keyFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Unable to connect to open the necessary files for SSL/TLS, there's noting to do...";
        exit(EXIT_FAILURE);
    }
    
    // We open and load the certificate and the key
    QSslCertificate certificate(&certFile, QSsl::Pem);
    QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
    certFile.close();
    keyFile.close();
    
    // We create a SSL/TLS configuration and tell the server to use it
    sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConfiguration.setLocalCertificate(certificate);
    sslConfiguration.setPrivateKey(sslKey);
    sslConfiguration.setProtocol(QSsl::SecureProtocols);
    _server.setSslConfiguration(sslConfiguration);
    
    // Now we start the listening of the network
    if (_server.listen(QHostAddress::AnyIPv4, port))
    {
        qDebug() << "Chator listening on port" << port;
        connect(&_server, &QWebSocketServer::newConnection, this, &Listener::newConnection);
        connect(&_server, &QWebSocketServer::sslErrors, this, &Listener::SSLErrors);
    }
    
    // If we cannot listen to the network, this is a critical error, we can't continue
    else
    {
        qDebug() << "Unable to connect to listen to network, there's noting to do...";
        exit(EXIT_FAILURE);
    }
}

Listener::~Listener()
{
    // Gracefully close the socket and destroy every remaining pending connection
    _server.close();
    qDeleteAll(_clients.begin(), _clients.end());
}

void Listener::newConnection()
{
    // We get the new peer and add it to the list
    QWebSocket *newSocket = _server.nextPendingConnection();
    ChatorClient* client = new ChatorClient(*newSocket);
    _clients << client;
    
    // Bind its signals to the right receptor
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(client, SIGNAL(binaryMessageReceived(const QByteArray&)), &_interpretor, SLOT(processData(const QByteArray&)));
}

void Listener::disconnected()
{
    // We have to remove the client from the list
    ChatorClient* client = (ChatorClient*) sender();
    _clients.removeAll(client);
    
    if (client)
    {
        // If the client was logged in, we have to disconnect it in the chat
        if (client->logged) {_controllerInput.disconnect(client->id, (QObject*) client);}
        
        // We can finally delete it
        delete client;
    }
}

void Listener::SSLErrors(const QList<QSslError> &)
{
    qDebug() << "Ssl errors occurred...";
}
