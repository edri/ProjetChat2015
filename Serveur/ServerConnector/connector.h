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
    Connector();
    void connectToServer(QString url);
    void send(const QByteArray& data);
    bool connected();
    
    private slots:
    void sslError(const QList<QSslError>& errors);
    
    private:
    QWebSocket _socket;
    bool _isConnected;
};

#endif
