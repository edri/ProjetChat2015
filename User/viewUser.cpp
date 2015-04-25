#include "viewUser.h"
#include "ui_viewUser.h"

ViewUser::ViewUser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewUser)
{
    ui->setupUi(this);
    ui->ldt_password->setValidator(new QIntValidator());
    // Initialiser une nouvelle vue pour l'inscription
    this->_viewInscription = new ViewInscription();
}

ViewUser::~ViewUser()
{
    delete _viewInscription;
    delete ui;
}

void ViewUser::on_btn_connexion_clicked()
{
    // Envoyer les données

    // Valider manuellement: les champs doivent être remplis
    if(ui->btn_connexion->text().isEmpty()) {
        // Afficher un message d'erreur
    }

    // Récupérer les identifiants

    // Emettre le signal
    emit requestGetIds();
}

void ViewUser::on_btn_inscription_clicked()
{
    // Ouvrir la fenêtre inscription
    _viewInscription->show();
}


QString ViewUser::getUsername()
{
    return ui->ldt_Username->text();
}

QString ViewUser::getPassword()
{
    return ui->ldt_password->text();
}

QString ViewUser::getPort() {
    return ui->spinBox->text();

}

QString ViewUser::getIpAddress() {
    return ui->ldt_server->text();
}
