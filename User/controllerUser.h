#ifndef CONTROLLERUSER_H
#define CONTROLLERUSER_H

#include <QString>
#include "viewUser.h"
#include "../ModeleChator/modelChator.h"
#include "../Serveur/controllerInput/clientControllerInput.h"
#include "../Serveur/Interpretor/interpretor.h"
#include "../Serveur/ServerConnector/connector.h"
#include "../Serveur/controllerOutput/controllerOutput.h"
#include "../Chat/controllerChat.h"

class ClientControllerInput;
class ControllerChat;


class ControllerUser : public QObject
{

    Q_OBJECT

private:

    ViewUser* _view;
    ViewUser* _viewInscription;
    ModelChator* _model;

    ClientControllerInput* cci;
    Interpretor* i;
    ClientConnector* cc;
    ControllerOutput* co;
    ControllerChat* _controllerChat;

    ModelUser* _currentUser;


public:
    ControllerUser(ModelChator* model, ModelUser* currentUser, ClientControllerInput* cci, Interpretor* i,
                   ClientConnector* cc, ControllerOutput* co, ControllerChat* controllerChat);
    ~ControllerUser();

    // Afficher la vue
    void showView() const;

    //
    void infoUser(ModelUser& user);



public slots:
    void connectToServer() const;
    void auth() const;

};

#endif // CONTROLLERUSER_H
