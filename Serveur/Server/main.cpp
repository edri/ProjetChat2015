#include <QApplication>
#include <QString>
#include "listener.h"
#include "controllerDB.h"

int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);
    
    Listener listener(1234);
    ControllerDB cdb("db.sqlite");
    
    return a.exec();
}
