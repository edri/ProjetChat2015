/*
     * Implements connector.h
*/

#include <QtCore/QDebug>
#include "connector.h"
#include <QUrl>
#include <QMessageBox>

ClientConnector::ClientConnector() : _isConnected(false)
{
    // The constructors binds the signals of the sockets to the internal methods
    connect(&_socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(&_socket, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(sslErrors(const QList<QSslError>&)));
    connect(&_socket, SIGNAL(binaryMessageReceived(const QByteArray&)), this, SIGNAL(binaryMessageReceived(const QByteArray&)));
    connect(&_socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
}

void ClientConnector::connectToServer(QString url)
{
    if (!_isConnected)
    {
        _socket.open(QUrl("wss://" + url));
    }
}

void ClientConnector::send(const QByteArray& data)
{
    if (_isConnected)
    {
        _socket.sendBinaryMessage(data);
    }
}

void ClientConnector::sslErrors(const QList<QSslError>& errors)
{
    // We build a messagebox that will display the text of the error and the associated certificate
    QMessageBox agreeMessageBox(QMessageBox::Question, tr("Erreur SSL/TLS"), "");
    
    
    // We display every error
    for (QSslError error : errors)
    {
        agreeMessageBox.setText(tr("Une erreur SSL/TLS est survenue lors de la connexion au serveur: ") + _socket.peerName());
        agreeMessageBox.setInformativeText(error.errorString() + "\n\n" + tr("Voulez-vous continuer?"));
        agreeMessageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        agreeMessageBox.setDefaultButton(QMessageBox::Yes);
        agreeMessageBox.setDetailedText(error.certificate().toText());
        
        // If the users doesn't accept one, the connection is dropped
        if (agreeMessageBox.exec() != QMessageBox::Yes) {_socket.abort(); return;}
    }
    
    // The user has accepted every SSL/TLS error, we can continue
    _socket.ignoreSslErrors();
}

void ClientConnector::connected()
{
    _isConnected = true;
    emit connectionSuccessful();
}
