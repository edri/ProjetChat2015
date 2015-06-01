#include "viewabout.h"
#include "ui_viewabout.h"

ViewAbout::ViewAbout(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::viewAbout),
    _parent(parent)
{
    _ui->setupUi(this);

    // Add the application's logo in the QGraphicsView componant of the window.
    QPixmap logo;
    logo.load(":/icons/img/chator.png");
    QGraphicsScene* sceneLogo = new QGraphicsScene(_ui->grv_logo);
    sceneLogo->addPixmap(logo);
    _ui->grv_logo->setScene(sceneLogo);
    _ui->grv_logo->setRenderHint(QPainter::Antialiasing);

    // Add the heig's logo in the QGraphicsView componant of the window.
    QPixmap heig;
    heig.load(":/icons/img/heig.png");
    QGraphicsScene* sceneHeig = new QGraphicsScene(_ui->grv_heig);
    sceneHeig->addPixmap(heig);
    _ui->grv_heig->setScene(sceneHeig);
    _ui->grv_heig->setRenderHint(QPainter::Antialiasing);
}

ViewAbout::~ViewAbout()
{
    delete _ui;
    delete _parent;
}

void ViewAbout::closeEvent(QCloseEvent*)
{
    _parent->setEnabled(true);
}
