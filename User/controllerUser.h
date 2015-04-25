#ifndef CONTROLLERUSER_H
#define CONTROLLERUSER_H

#include <QString>
#include "viewUser.h"
#include "../ModeleChator/modelChator.h"
#include "../Serveur/controllerInput/controllerInput.h"
#include "../Serveur/Interpretor/interpretor.h"
#include "../Serveur/ServerConnector/connector.h"
#include "../Serveur/controllerOutput/controllerOutput.h"

class ControllerUser : public QObject
{

    Q_OBJECT

private:

    ViewUser* _view;

    ClientControllerInput* cci;
    Interpretor* i;
    ClientConnector* cc;
    ControllerOutput* co;

public:
    ControllerUser(ClientControllerInput* cci, Interpretor* i, ClientConnector* cc, ControllerOutput* co);
    ~ControllerUser();

    // Afficher la vue
    void showView() const;

public slots:
    void connectToServeur() const;
    void auth() const;

};

#endif // CONTROLLERUSER_H
