#include <QApplication>
#include <QString>
#include "listener.h"
#include "controllerDB.h"
#include "interpretor.h"
#include "controllerInput.h"

int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);
    
    Listener listener(1234);
    
    ControllerDB cdb("db.sqlite");
    
    ServerControllerInput sci;
    Interpretor i(sci);
    
    
    return a.exec();
}
