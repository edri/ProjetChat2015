#include <QApplication>
#include <QDateTime>
#include <QByteArray>
#include "interpretor.h"
#include "modelChator.h"

int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);
    Interpretor i;
    
    return a.exec();
}
