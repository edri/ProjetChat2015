#include <QtCore/QCoreApplication>
#include "connector.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Connector c();

    Q_UNUSED(c);

    return a.exec();
}
