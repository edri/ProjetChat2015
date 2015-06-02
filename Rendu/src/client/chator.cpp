/*
     * Created by Jan Purro
     *
     * This is the main programm for the client. His reponsability is to initialize
     * the different elements of the application such as models and
     * controllers. It's controller responsability to create the
     * corresponding views.
     *
*/

#include "userModule/viewUser.h"
#include "userModule/controllerUser.h"
#include "../common/models/modelChator.h"
#include "../common/controllerInput/controllerInput.h"
#include "../common/interpretor/interpretor.h"
#include "connectionModule/connector.h"
#include "connectionModule/controllerOutput.h"
#include "../common/cryptor/cryptor.h"
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
    Cryptor* cryptor = new Cryptor();

    // Create the controler which will create the view
    ControllerRoom* controllerRoom = new ControllerRoom (model, currentUser, controllerOutput, cryptor);
    ControllerChat* controllerChat = new ControllerChat(model, currentUser, controllerInput, interpretor, connector, controllerOutput, controllerRoom, cryptor);
    ControllerUser* controllerUser = new ControllerUser(model, currentUser, controllerInput, interpretor, connector, controllerOutput, controllerChat, cryptor);
    controllerInput->controllerUser(controllerUser);
    controllerInput->controllerChat(controllerChat);
    controllerInput->controllerRoom(controllerRoom);

    // Call the view
    controllerUser->showView();

    return a.exec();
}
