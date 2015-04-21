#include <QtCore/QCoreApplication>
#include <QThread>
#include "controllerOutput.h"
#include "interpretor.h"
#include "controllerInput.h"
#include "connector.h"
#include <iostream>
#include "test.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    Test t;
    

    return a.exec();
}
