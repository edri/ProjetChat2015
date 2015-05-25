#include <QtCore/QDebug>
#include "connector.h"
#include <QUrl>
#include <QMessageBox>

ClientConnector::ClientConnector() : _isConnected(false)
{
    connect(&_socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(&_socket, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(sslErrors(const QList<QSslError>&)));
    connect(&_socket, SIGNAL(binaryMessageReceived(const QByteArray&)), this, SIGNAL(binaryMessageReceived(const QByteArray&)));
}

void ClientConnector::connectToServer(QString url)
{
    if (!_isConnected)
    {
        qDebug() << "trying to connect to " << url;
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

void ClientConnector::connected()
{
    _isConnected = true;
    qDebug() << "connected";
    emit connectionSuccessful();
}

void ClientConnector::sslErrors(const QList<QSslError>& errors)
{
    QMessageBox agreeMessageBox(QMessageBox::Question, tr("Erreur SSL/TLS"), "");
    
    for (QSslError error : errors)
    {
        //qDebug() << "Erreur SSL: " << error.errorString();
        agreeMessageBox.setText(tr("Une erreur SSL/TLS est survenue lors de la connexion au serveur: ") + _socket.peerName());
        agreeMessageBox.setInformativeText(error.errorString() + "\n\n" + tr("Voulez-vous continuer?"));
        agreeMessageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        agreeMessageBox.setDefaultButton(QMessageBox::Yes);
        agreeMessageBox.setDetailedText(error.certificate().toText());
        
        if (agreeMessageBox.exec() != QMessageBox::Yes) {_socket.abort(); return;}
    }
    
    _socket.ignoreSslErrors();
}
