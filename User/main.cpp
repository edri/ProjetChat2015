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
    
    ModelChator* model = new ModelChator();
    ModelUser* currentUser = new ModelUser();
    ClientControllerInput* controllerInput = new ClientControllerInput();
    Interpretor* interpretor = new Interpretor(*controllerInput);
    ClientConnector* connector = new ClientConnector();
    ControllerOutput* controllerOutput = new ControllerOutput(*connector, *interpretor);

    ControllerChat* controllerChat = new ControllerChat(model, currentUser, controllerInput, interpretor, connector, controllerOutput);
    controllerInput->controllerChat(controllerChat);

    // Create the controler which will create the view
    ControllerUser* controllerUser = new ControllerUser(model, currentUser, controllerInput, interpretor, connector, controllerOutput, controllerChat);
    ControllerRoom* controllerRoom = new ControllerRoom (model, currentUser, controllerOutput);
    controllerInput->controllerUser(controllerUser);
    controllerInput->controllerChat(controllerChat);
    controllerInput->controllerRoom(controllerRoom);

    // Call the view
    controllerUser->showView();

    return a.exec();
}
