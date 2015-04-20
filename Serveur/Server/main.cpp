#include <QApplication>
#include "listener.h"

int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);
    
    Listener listener(1234);
    
    return a.exec();
}
