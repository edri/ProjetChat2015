#include <QApplication>
#include <QString>
#include "listener.h"
#include "controllerDB.h"
#include "../Interpretor/interpretor.h"
#include "serverControllerInput.h"
#include "controllerRoom.h"
#include "controllerUser.h"
#include "../../ModeleChator/chatorConstants.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    ControllerDB cdb(DATABASE_FILE_NAME);
    
    ControllerRoom cr(cdb);
    ControllerUser cu(cdb);
    cu._room = &cr;
    cr._user = &cu;
    
    ServerControllerInput sci(cu, cr);
    Interpretor i(sci);
    
    cu._interpretor = &i;
    cr._interpretor = &i;
    
    Listener listener(PORT_TO_LISTEN, i, sci);
    
    return a.exec();
}
