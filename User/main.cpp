#include "viewUser.h"
#include "controllerUser.h"
#include "../ModeleChator/modelChator.h"
#include "../Serveur/controllerInput/controllerInput.h"
#include "../Serveur/Interpretor/interpretor.h"
#include "../Serveur/ServerConnector/connector.h"
#include "../Serveur/controllerOutput/controllerOutput.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ClientControllerInput* cci = new ClientControllerInput();
    Interpretor* i = new Interpretor(*cci);
    ClientConnector* cc = new ClientConnector();
    ControllerOutput* co = new ControllerOutput(*cc, *i);
    ModelChator* model = new ModelChator();
    ModelUser currentUser;
    ControllerChat* controllerChat = new ControllerChat(model, &currentUser, cci, i, cc, co);

    // Create the controler which will create the view
    ControllerUser* controllerUser = new ControllerUser(model, &currentUser, cci, i, cc, co, controllerChat);
    cci->controllerUser(controllerUser);
    // Il faut encore initialiser les autres controlleurs du cci.

    // Call the view
    controllerUser->showView();

    return a.exec();
}
