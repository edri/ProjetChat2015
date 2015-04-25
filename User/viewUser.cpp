#include "viewUser.h"
#include "ui_viewUser.h"

ViewUser::ViewUser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewUser)
{
    ui->setupUi(this);
    ui->ldt_password->setValidator(new QIntValidator());
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
    // Send the data

    // manually valid : The field must be filled.
    if(ui->btn_connexion->text().isEmpty()) {
        // Show an error message
    }

    // Get the login

    // send a signal
    emit requestGetIds();
}

void ViewUser::on_btn_inscription_clicked()
{
    // Open the inscription window
    _viewInscription->show();
    // Disable the main windows
    setEnabled(false);
    _viewInscription->setEnabled(true);
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
