#include "viewUser.h"
#include "ui_viewUser.h"

ViewUser::ViewUser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewUser)
{
    ui->setupUi(this);
}

ViewUser::~ViewUser()
{
    delete ui;
}

void ViewUser::on_btn_connexion_clicked()
{
    // Envoyer les données
}

void ViewUser::on_btn_inscription_clicked()
{
    // Ouvrir la fenêtre inscription
}
