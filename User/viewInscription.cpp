#include "viewInscription.h"
#include "ui_viewInscription.h"
#include <QDebug>

ViewInscription::ViewInscription(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::viewInscription)
{
    ui->setupUi(this);
    ui->btn_question->setEnabled(false);
    ui->btn_question->setToolTip(passwordRequirement);
}

ViewInscription::~ViewInscription()
{
    delete ui;
}

void ViewInscription::on_btn_path_clicked()
{
    // Browse the files in order to find a picture
    QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString(), "Images (*.png *.gif *.jpg *.jpeg)");

    ui->ldt_profilPicture->setText(fichier);
    // QMessageBox::information(this, "Fichier", "Vous avez sélectionné :\n" + fichier);

}

void ViewInscription::on_btn_inscription_clicked()
{
    /* TODO
     * Regrouper dans une fonction les vérification des champs, cette dernière retourne true si tout est ok
     * Envoi des données au serveur et réception de réponses de ce dernier
    */
    ui->lbl_info->setText("");

    if(verifyFields())
    {
        // Send the data
        emit requestGetNewUser();

        /* TODO

        //Warn the controler that data are ready to be send to the server
        // send a signal

        //The server verify if the username is already used
        if(true) {
          ui->lbl_info->setText("<font color='red'>Ce nom d'utilisateur est déjà utilisé.</font>");
        }

        // User added succesfully
        // Afficher un popup indiquant que l'inscription est réussie

        QMessageBox info;
        info.setText("Votre compte a été créé avec succès");
        info.exec();

        // Réactiver la fenêtre de connection si tout s'est bien passé

        */
        ui->lbl_info->setText("Prêt à être envoyé au serveur");
    }
}

bool ViewInscription::verifyFields()
{
    // Regular expressions
    QRegExp passwordRestriction("^(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])(?=.*[^a-z^A-Z^0-9]).{8,}$");
    QRegExp userNameRestriction("^[a-zA-Z0-9_ -]*$");

    // Verify the fields
    if(ui->ldt_userName->text().isEmpty() || ui->ldt_password->text().isEmpty() || ui->ldt_passwordConf->text().isEmpty() ||
       ui->ldt_password->text().isEmpty() || ui->ldt_passwordConf->text().isEmpty())
    {
        ui->lbl_info->setText("<font color='red'>Veuillez mentionnez tous les champs requis.</font>");
        return false;
    }
    else if(!userNameRestriction.exactMatch(ui->ldt_userName->text()))
    {
        ui->lbl_info->setText("<font color='red'>Le nom d'utilisateur doit contenir des chiffres, des lettres ou - et _</font>");
        return false;
    }
    else if(ui->ldt_password->text() != ui->ldt_passwordConf->text())
    {
        ui->lbl_info->setText("<font color='red'>Le mot de passe ne correspond pas.</font>");
        return false;
    }
    else if(ui->ldt_password->text() == ui->ldt_userName->text())
    {
        ui->lbl_info->setText("<font color='red'>Votre mot de passe doit être différent de votre nom d'utilisateur.</font>");
        return false;
    }
    else if(!passwordRestriction.exactMatch(ui->ldt_password->text()))
    {
        ui->lbl_info->setText("<font color='red'>" + passwordRequirement + "</font>");
        return false;
    }
    else if(!ui->ldt_profilPicture->text().isEmpty() && !verifyProfileImage())
    {
        ui->lbl_info->setText("<font color='red'>Image spécifiée incorrecte</font>");
        return false;
    }
    else
        return true;
}

bool ViewInscription::verifyProfileImage()
{
    QString extension = ui->ldt_profilPicture->text().right(4).toLower();
    QFile file(ui->ldt_profilPicture->text());

    return(file.exists()&&(extension == ".png" || extension == ".gif" || extension == ".jpg" || extension == ".jpeg"));
}

void ViewInscription::closeEvent(QCloseEvent *)
{
    // Enable connexion window once you close the inscription window
    //ViewInscription::QMainWindow.setDisabled(false);
}


QString ViewInscription::getFirstName() const
{
    return ui->ldt_firstName->text();
}

QString ViewInscription::getLastName() const
{
    return ui->ldt_lastName->text();
}

QString ViewInscription::getUserName() const
{
    return ui->ldt_userName->text();
}

QString ViewInscription::getPassword() const
{
    return ui->ldt_password->text();
}

QImage ViewInscription::getProfileImage() const
{
    return QImage(ui->ldt_profilPicture->text());
}
