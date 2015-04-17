#ifndef CONTROLLERCHAT
#define CONTROLLERCHAT

#include <QString>

#include "viewChat.h"
#include "../ModeleChator/modelChator.h"

class ControllerChat : public QObject
{
    Q_OBJECT
private:
    ViewChat* view;
    ModelChator* model;

public:
    ControllerChat(ModelChator* model);
    ~ControllerChat();

    void showView();

public slots :
    void loadRooms(const quint32 idUser);
    //void loadMessages(const quint32 idRoom);
    void addMessage(const quint32 idUser, const quint32 idRoom, const QString message);

};

#endif // CONTROLLERCHAT
