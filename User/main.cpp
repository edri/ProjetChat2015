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

    // Créer le contrôleur qui créera la vue
    ControllerUser* controller = new ControllerUser(cci, i, cc, co);
    // Lancer la vue
    controller->showView();



    // Lance l'affichage des fenêtres + renvoie le
    // signal de terminaison
    return a.exec();
}
