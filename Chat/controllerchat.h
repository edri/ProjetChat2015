#ifndef CONTROLLERCHAT
#define CONTROLLERCHAT

#include <QString>

#include "viewChat.h"
#include "../ModeleChator/modelChator.h"

class ControllerChat : public QObject
{
    Q_OBJECT
private:
    ViewChat* _view;
    ModelChator* _model;
    ModelUser* _currentUser;

public:
    ControllerChat(ModelChator* model, ModelUser* currentUser);
    ~ControllerChat();

    void showView();

public slots :
    void loadRooms(const quint32 idUser);

};

#endif // CONTROLLERCHAT
