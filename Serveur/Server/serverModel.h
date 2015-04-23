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

    quint32 id;
    bool logged;
    QWebSocket& socket;
    QList<ChatorRoom*> rooms;
    
    signals:
    void disconnected();
    void binaryMessageReceived(const QByteArray& message);
};

struct ChatorRoom
{
    quint32 id;
    quint32 admin;
    QList<ChatorClient*> clients;
};

#endif
