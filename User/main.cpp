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

    // Create the controler which will create the view
    ControllerUser* controller = new ControllerUser(cci, i, cc, co);
    // Call the view
    controller->showView();

    return a.exec();
}
