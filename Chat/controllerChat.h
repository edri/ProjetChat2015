#ifndef CONTROLLERCHAT
#define CONTROLLERCHAT

#include <QString>

#include "viewChat.h"
#include "../ModeleChator/modelChator.h"
#include "../Serveur/controllerInput/clientControllerInput.h"
#include "../Serveur/Interpretor/interpretor.h"
#include "../Serveur/ServerConnector/connector.h"
#include "../Serveur/controllerOutput/controllerOutput.h"

class ClientControllerInput;

class ControllerChat : public QObject
{
    Q_OBJECT
private:
    ViewChat* _view;
    ModelChator* _model;
    ModelUser* _currentUser;
    ClientControllerInput* _cci;
    Interpretor* _i;
    ClientConnector* _cc;
    ControllerOutput* _co;

public:
    ControllerChat(ModelChator* model, ModelUser* currentUser, ClientControllerInput* cci,
                   Interpretor* i, ClientConnector* cc, ControllerOutput* co);
    ~ControllerChat();

    void showView() const;
    void loadUser(ModelUser& user) const;
    void loadRoom(ModelRoom& room) const;
    void receiveMessage(ModelMessage& message) const;

public slots :
    void loadUserRooms() const;
    void loadRoomMessages(const quint32 idRoom) const;
    void sendMessage() const;

};

#endif // CONTROLLERCHAT
