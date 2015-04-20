#include <QApplication>
#include <QDateTime>
#include <QByteArray>
#include "interpretor.h"
#include "modelChator.h"
#include "controllerInput.h"

int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);
    
    ClientControllerInput cci;
    Interpretor i(&cci);
    
    return a.exec();
}
