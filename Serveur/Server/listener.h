#ifndef LISTENER_H
#define LISTENER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QtNetwork/QSslError>
#include <QWebSocketServer>
#include "../Interpretor/interpretor.h"
#include "serverModel.h"

class Listener : public QObject
{
    Q_OBJECT
public:
    Listener(quint16 port, Interpretor& interpretor);
    ~Listener();

public slots:
    void newConnection();
    void disconnected();
    void SSLErrors(const QList<QSslError> &errors);

private:
    QWebSocketServer _server;
    QList<ChatorClient*> _clients;
    Interpretor& _interpretor;
};

#endif
