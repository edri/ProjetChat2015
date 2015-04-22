#include <QApplication>
#include <QString>
#include "listener.h"
#include "controllerDB.h"
#include "../Interpretor/interpretor.h"
#include "serverControllerInput.h"
#include "controllerRoom.h"
#include "controllerUser.h"

int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);
    
    Listener listener(1234);
    
    ControllerDB cdb("db.sqlite");
    
    ControllerRoom cr(cdb);
    ControllerUser cu(cdb);
    
    ServerControllerInput sci;
    Interpretor i(sci);
    
    return a.exec();
}
