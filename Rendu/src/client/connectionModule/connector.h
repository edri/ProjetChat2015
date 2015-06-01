/*
     * Created by Benoist Wolleb
     *
     * Handle customers connection between server and customer
     * by using sockets. And allows transmissions of packets previously
     * serilalized by the interpretor.
*/

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
    
    /*
     * This method sends the provided data into the socket, if connected.
     * -data : the binary data to send.
     */
    virtual void send(const QByteArray& data) = 0;
};

// This class implements the Connector interface.
class ClientConnector : public Connector
{
    Q_OBJECT
    
    public:
    
    // Constructor
    ClientConnector();
    
    /*
     * This method tries to connect to a server at the specified url.
     * -url : the url to connect to (can be an IP or a name).
     */
    void connectToServer(QString url);
    
    // Concrete implementation of the Connector interface.
    void send(const QByteArray& data);
    
    
    signals:
    
    // This signal is emitted when the socket has successfully connected.
    void connectionSuccessful();
    
    // This signal is emitted when the socket has received a complete binary message to process.
    void binaryMessageReceived(const QByteArray& message);
    
    // This signal is emitted when the socket disconnects.
    void disconnected();
    
    
    private slots:
    
    /*
     * This method displays the SSL/TLS errors that occured while connecting.
     * -errors : the errors to display
     */
    void sslErrors(const QList<QSslError>& errors);
    
    /*
     * This method is called when the socket has successfully connected.
     */
    void connected();
    
    private:
    QWebSocket _socket;
    bool _isConnected;
};

#endif
