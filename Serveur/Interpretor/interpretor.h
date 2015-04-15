#ifndef INTERPRETOR_H
#define INTERPRETOR_H

#include "modelChator.h"
//#include <QObject>
#include <QByteArray>

//class Interpretor : public QObject        // Not sure yet if the Interpretor has to be a subclass of QObject (will it have signals/slots?)
class Interpretor
{
    public:
    void sendMessage(const ModelMessage& message);
    void login(const QString& pseudo, const QString& pass);
    void createAccount(const ModelUser& user);
    void sendInfoUser(const ModelUser& user);
    void sendError(const QString& text);
    
    void processData(const QByteArray& data);
    
    //private:
    // Pointeur sur l'objet réseau
};

#endif
