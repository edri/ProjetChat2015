#include <QApplication>
#include <QDateTime>
#include "interpretor.h"
#include "modelChator.h"

int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);
    Interpretor i;
    
    ModelMessage m(123, 756, QDateTime::currentDateTime(), "message de test");
    
    return a.exec();
}
