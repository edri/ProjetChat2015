#include "viewUser.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ViewUser w;
    w.show();

    // Lance l'affichage des fenêtres + renvoie le
    // signal de terminaison
    return a.exec();
}
