/*
     * Implements serverModel.h
*/

#include "serverModel.h"

ChatorClient::ChatorClient(QWebSocket& socket) : socket(socket)
{
    id = 0;
    logged = false;
    
    // We forward the signals from the internal socket
    connect(&socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    connect(&socket, SIGNAL(binaryMessageReceived(const QByteArray&)), this, SIGNAL(binaryMessageReceived(const QByteArray&)));
}

ChatorClient::~ChatorClient()
{
    //delete &socket;
}
