#include <QApplication>
#include "controllerChat.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ControllerChat c;
    c.showView();

    return a.exec();
}
