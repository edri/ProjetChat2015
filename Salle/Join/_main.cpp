#include <QApplication>
#include "viewJoin.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ViewJoin w;
    w.show();
    
    return a.exec();
}
