/*
     * Created by Benoist Wolleb
     *
     * Implements serverModel.h
     *
*/

#include "serverModel.h"

ChatorClient::ChatorClient(QWebSocket& socket) : socket(socket)
{
    id = 0;
    logged = false;
    
    connect(&socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    connect(&socket, SIGNAL(binaryMessageReceived(const QByteArray&)), this, SIGNAL(binaryMessageReceived(const QByteArray&)));
}

ChatorClient::~ChatorClient()
{
    qDebug() << "suppr client...";
    //delete &socket;
    qDebug() << "Fin suppr...";
}
