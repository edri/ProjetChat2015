#include <QtCore/QCoreApplication>
#include "controllerInput.h"
#include "clientControllerInput.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ClientControllerInput c();

    Q_UNUSED(c);

    return a.exec();
}
