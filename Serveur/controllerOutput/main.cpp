#include <QtCore/QCoreApplication>
#include "controllerOutput.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ControllerOutput c();

    Q_UNUSED(c);

    return a.exec();
}
