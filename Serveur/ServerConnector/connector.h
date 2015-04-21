#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QByteArray>
#include <QString>

class Connector : public QObject
{
    Q_OBJECT
    
    public:
    virtual void send(const QByteArray& data) = 0;
};

class ClientConnector : public Connector
{
    Q_OBJECT
    
    public:
    ClientConnector();
    void connectToServer(QString url);
    void send(const QByteArray& data);
    
    signals:
    void connectionSuccessful();
    
    private slots:
    void sslErrors(const QList<QSslError>& errors);
    void connected();
    
    private:
    QWebSocket _socket;
    bool _isConnected;
};

#endif
