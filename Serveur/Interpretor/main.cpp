#include <QApplication>
#include <QDateTime>
#include <QByteArray>
#include "interpretor.h"
#include "modelChator.h"

int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);
    Interpretor i;
    
    ModelMessage m(123, 756, QDateTime::currentDateTime(), "message de test");
    i.sendMessage(m);
    
    QByteArray data = i.out;
    Interpretor i2;
    i2.processData(data);
    
    return a.exec();
}
