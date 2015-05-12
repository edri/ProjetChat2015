#include "viewprofileedition.h"
#include "ui_viewprofileedition.h"

ViewProfilEedition::ViewProfilEedition(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewProfilEedition)
{
    ui->setupUi(this);
}

ViewProfilEedition::~ViewProfilEedition()
{
    delete ui;
}
