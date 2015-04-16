#include <QApplication>
#include "controllerRoom.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ControllerRoom w;
    w.show();
    
    return a.exec();
}
