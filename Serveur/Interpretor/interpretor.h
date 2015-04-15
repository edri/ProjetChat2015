#ifndef INTERPRETOR_H
#define INTERPRETOR_H

#include "modelChator.h"
#include "packet.h"
//#include <QObject>
#include <QByteArray>
#include <array>

//class Interpretor : public QObject        // Not sure yet if the Interpretor has to be a subclass of QObject (will it have signals/slots?)
class Interpretor
{
    public:
    void sendMessage(const ModelMessage& message);
    void login(const QString& pseudo, const QString& pass);
    void createAccount(const ModelUser& user);
    void sendInfoUser(const ModelUser& user);
    void sendError(const QString& text);
    void join(const quint32 idUser, const quint32 idRoom);
    void leave(const quint32 idUser, const quint32 idRoom);
    void disconnect(const quint32 idUser);
    
    void processData(const QByteArray& data);
    
    private:
    // Pointeur sur l'objet réseau
    
    // test
    void test(std::array<void (*) (const QByteArray&), (quint32) MessageType::ERROR> fs);
    std::array<void (*) (const QByteArray&), (quint32) MessageType::ERROR> bridge;
};

#endif
