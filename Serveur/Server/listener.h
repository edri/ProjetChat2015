#ifndef LISTENER_H
#define LISTENER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QtNetwork/QSslError>
#include <QWebSocketServer>

class Listener : public QObject
{
    Q_OBJECT
public:
    Listener(quint16 port);
    ~Listener();

public slots:
    void newConnection();
    void receive(QByteArray message);
    void disconnected();
    void SSLErrors(const QList<QSslError> &errors);

private:
    QWebSocketServer _server;
    QList<QWebSocket *> _clients;
};

#endif
