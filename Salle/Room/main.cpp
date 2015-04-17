#include <QApplication>
#include "controllerRoom.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ControllerRoom w;
    w.showRoom();
    
    return a.exec();
}
