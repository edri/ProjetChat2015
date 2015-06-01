/*
     * This class listens to the network for new peers.
*/

#ifndef LISTENER_H
#define LISTENER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QtNetwork/QSslError>
#include <QWebSocketServer>
#include "../../common/interpretor/interpretor.h"
#include "serverModel.h"
#include "../controllers/serverControllerInput.h"

class Listener : public QObject
{
    Q_OBJECT
    
    public:
    // Constructor
    Listener(quint16 port, Interpretor& interpretor, ServerControllerInput& controllerInput);
    // Destructor
    ~Listener();

    public slots:
    // This method is called when a new peer is connecting
    void newConnection();
    
    // This method is called when a socket disconnects
    void disconnected();
    
    // This method is called when SSL/TLS errors occur (shouldn't be the case)
    void SSLErrors(const QList<QSslError> &errors);

    private:
    QWebSocketServer _server;
    QList<ChatorClient*> _clients;
    Interpretor& _interpretor;
    ServerControllerInput& _controllerInput;
};

#endif
