#include <QApplication>
#include "join.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Join w;
    w.show();
    
    return a.exec();
}
