#include "serverModel.h"

ChatorClient::ChatorClient(QWebSocket& socket) : socket(socket)
{
    id = 0;
    logged = false;
    
    connect(&socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    connect(&socket, SIGNAL(binaryMessageReceived(const QByteArray& message)), this, SIGNAL(binaryMessageReceived(const QByteArray& message)));
}

ChatorClient::~ChatorClient()
{
    socket.close();
    delete &socket;
}
