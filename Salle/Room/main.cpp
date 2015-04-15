#include <QApplication>
#include "roomController.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RoomController w;
    w.show();
    
    return a.exec();
}
