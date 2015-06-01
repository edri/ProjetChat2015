/*
     * Created by Benoist Wolleb
     *
     * This is the main programm for the server. His reponsability is to
     * initialize the different elements of the application such as the
     * controllers and the listener.
     *
*/

#include <QApplication>
#include <QString>
#include "utils/listener.h"
#include "controllers/controllerDB.h"
#include "../common/interpretor/interpretor.h"
#include "controllers/serverControllerInput.h"
#include "controllers/controllerRoom.h"
#include "controllers/controllerUser.h"
#include "../common/models/chatorConstants.h"

int main(int argc, char *argv[])
{
    // We have to build the elements in the right order.
    
    QApplication QtApplication(argc, argv);
    
    ControllerDB controllerDb(DATABASE_FILE_NAME);
    
    ControllerRoom controllerRoom(controllerDb);
    ControllerUser controllerUser(controllerDb);
    controllerUser._room = &controllerRoom;
    controllerRoom._user = &controllerUser;
    
    ServerControllerInput serverControllerInput(controllerUser, controllerRoom);
    Interpretor interpretor(serverControllerInput);
    
    controllerUser._interpretor = &interpretor;
    controllerRoom._interpretor = &interpretor;
    
    Listener listener(PORT_TO_LISTEN, interpretor, serverControllerInput);
    
    return QtApplication.exec();
}
