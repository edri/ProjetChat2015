#ifndef CONTROLLERCHAT
#define CONTROLLERCHAT

#include <QString>

#include "viewChat.h"
#include "../ModeleChator/modelChator.h"
#include "../Serveur/controllerInput/controllerInput.h"
#include "../Serveur/Interpretor/interpretor.h"
#include "../Serveur/ServerConnector/connector.h"
#include "../Serveur/controllerOutput/controllerOutput.h"

class ControllerChat : public QObject
{
    Q_OBJECT
private:
    ViewChat* _view;
    ModelChator* _model;
    ModelUser* _currentUser;
    ClientControllerInput *cci;
    Interpretor* i;
    ClientConnector* cc;
    ControllerOutput* co;

public:
    ControllerChat(ModelChator* model, ModelUser* currentUser);
    ~ControllerChat();

    void showView() const;

public slots :
    void loadRooms(const quint32 idUser) const;
    void loadRoomMessages(const quint32 idRoom) const;
    void sendMessage() const;
    void auth();

};

#endif // CONTROLLERCHAT
