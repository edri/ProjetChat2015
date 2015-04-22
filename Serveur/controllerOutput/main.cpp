#include <QtCore/QCoreApplication>
#include <QThread>
#include "controllerOutput.h"
#include "../Interpretor/interpretor.h"
#include "../controllerInput/controllerInput.h"
#include "../ServerConnector/connector.h"
#include <iostream>
#include "test.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    Test t;
    

    return a.exec();
}
