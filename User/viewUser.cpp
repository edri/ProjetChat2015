/*
     * Created by Mélanie Huck
     * Edited by Bastien Rouiller
     *
     * Implements viewUser.h
     *
*/

#include "viewUser.h"
#include "ui_viewUser.h"

ViewUser::ViewUser(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::ViewUser)
{
    _ui->setupUi(this);
    //ui->ldt_password->setValidator(new QIntValidator());
    // Initialize a new view for the inscription
    _viewInscription = new ViewInscription(this);

    _timerConnexion = new QTimer(this);
    connect(_timerConnexion, SIGNAL(timeout()), this, SLOT(errorServerUnreachable()));
}

ViewUser::~ViewUser()
{
    delete _viewInscription;
    delete _ui;
}

void ViewUser::on_btn_connexion_clicked()
{
    // Vérifier que les champs ne soient pas vides
    if(_ui->ldt_userName->text().isEmpty() || _ui->ldt_password->text().isEmpty() || _ui->ldt_server->text().isEmpty())
    {
        // Afficher un message d'erreur
        _ui->lbl_info->setText("<font color='red'>Veuillez mentionnez tous les champs requis.</font>");
    }
    else
    {
        _ui->lbl_info->setText("Connexion au serveur...");
        setDisabled(true);

        _timerConnexion->start(15000);
        // Send the data
        emit requestGetIds(true);
        // Les données doivent être confirmées
    }
}

void ViewUser::on_btn_inscription_clicked()
{
    // Vérifier que les champs ne soient pas vides
    if(_ui->ldt_server->text().isEmpty() || _ui->spn_port->text().isEmpty())
    {
        // Afficher un message d'erreur
        _ui->lbl_info->setText("<font color='red'>Veuillez mentionnez un serveur et un port.</font>");
    }
    else
    {
        _ui->lbl_info->setText("Connexion au serveur...");
        setDisabled(true);

        _timerConnexion->start(15000);
        // Connect to server in order to create a new account
        emit requestGetIds(false);
    }
}


QString ViewUser::getUsername() const
{
    return _ui->ldt_userName->text();
}

QString ViewUser::getPassword() const
{
    return _ui->ldt_password->text();
}

QString ViewUser::getPort() const
{
    return _ui->spn_port->text();
}

QString ViewUser::getIpAddress() const
{
    return _ui->ldt_server->text();
}

 ViewInscription* ViewUser::getViewInscription() const
{
    return _viewInscription;
}

void ViewUser::setInfoText(QString information)
{
    _ui->lbl_info->setText(information);
}

void ViewUser::authError()
{
    QMessageBox::critical(this, tr("Erreur d'authentification"),
                          tr("Vous devez entrer un identifiant et un mot de passe corrects."),
                          QMessageBox::Ok);
    _ui->lbl_info->clear();
    setEnabled(true);
}

void ViewUser::stopTimer()
{
    _timerConnexion->stop();
}

void ViewUser::errorServerUnreachable()
{
    _timerConnexion->stop();
    QMessageBox::critical(this, tr("Erreur de connexion"),
                          tr("Impossible de se connecter au serveur. Veuillez vérifier les informations de connexion, et si le problème persiste, réessayez dans un moment."),
                          QMessageBox::Ok);
    _ui->lbl_info->clear();
    setEnabled(true);
}
