/*
     * These structures are used to wrap every client socket in order to add
     * precious informations such as the ID of the user and the pointers to its
     * rooms. Il allows us to quickly browse the structure and send data to
     * every clients in a room in an efficient way.
*/

#ifndef CHATOR_MODEL_H
#define CHATOR_MODEL_H

#include <QtGlobal>
#include <QWebSocket>
#include <QList>
#include <QByteArray>

struct ChatorRoom;

class ChatorClient : public QObject
{
    Q_OBJECT
    
    public:
    ChatorClient(QWebSocket& socket);
    ~ChatorClient();
    
    // Useful data
    quint32 id;
    bool logged;
    QWebSocket& socket;
    QSet<ChatorRoom*> rooms;
    
    // We forward those sognals from the internal socket
    signals:
    void disconnected();
    void binaryMessageReceived(const QByteArray& message);
};

struct ChatorRoom
{
    quint32 id;
    QSet<ChatorClient*> clients;
};

#endif
