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
#include "../Cryptor/cryptor.h"

class ClientControllerInput;
class ControllerChat;


class ControllerUser : public QObject
{

    Q_OBJECT

private:

    ViewUser* _view;
    ModelChator* _model;

    ClientControllerInput* _cci;
    Interpretor* _i;
    ClientConnector* _cc;
    ControllerOutput* _co;
    ControllerChat* _controllerChat;

    Cryptor* _cryptor;

    ModelUser* _currentUser;

    bool _fromBtnConnection;


public:
    ControllerUser(ModelChator* model, ModelUser* currentUser, ClientControllerInput* cci, Interpretor* i,
                   ClientConnector* cc, ControllerOutput* co, ControllerChat* controllerChat, Cryptor* cryptor);
    ~ControllerUser();

    // Afficher la vue
    void showView() const;

    //
    void infoUser(ModelUser& user);
    void createUser(ModelUser& user);



public slots:
    void connectToServer(bool _fromBtnConnection);
    void auth() const;
    void inscriptionToServer() const;
    void editUser() const;

    void receiveSalt(Salt salt) const;

};

#endif // CONTROLLERUSER_H
