#include "viewUser.h"
#include "ui_viewUser.h"

ViewUser::ViewUser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewUser)
{
    ui->setupUi(this);
    //ui->ldt_password->setValidator(new QIntValidator());
    // Initialize a new view for the inscription
    this->_viewInscription = new ViewInscription(this);
}

ViewUser::~ViewUser()
{
    delete _viewInscription;
    delete ui;
}

void ViewUser::on_btn_connexion_clicked()
{
    // Vérifier que les champs ne soient pas vides
    if(ui->ldt_userName->text().isEmpty() || ui->ldt_password->text().isEmpty() || ui->ldt_server->text().isEmpty())
    {
        // Afficher un message d'erreur
        ui->lbl_info->setText("<font color='red'>Veuillez mentionnez tous les champs requis.</font>");
    }
    else
    {
        // Send the data
        emit requestGetIds(true);
        // Les données doivent être confirmées
    }
}

void ViewUser::on_btn_inscription_clicked()
{
    // TODO : Vérifier sur ce bouton ou lors de l'inscription si serveur atteignable
    // Send the data
    emit requestGetIds(false);
}


QString ViewUser::getUsername() const
{
    return ui->ldt_userName->text();
}

QString ViewUser::getPassword() const
{
    return ui->ldt_password->text();
}

QString ViewUser::getPort() const
{
    return ui->spinBox->text();

}

QString ViewUser::getIpAddress() const
{
    return ui->ldt_server->text();
}

 ViewInscription* ViewUser::getViewInscription() const
{
    return _viewInscription;
}

