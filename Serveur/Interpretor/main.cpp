#include <QApplication>
#include <QDateTime>
#include <QByteArray>
#include "interpretor.h"
#include "modelChator.h"
#include "controllerInput.h"
#include "connector.h"

int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);
    
    ClientControllerInput cci;
    //cc.connectToServer("localhost:1234");
    Interpretor i(&cci);
    
    return a.exec();
}
