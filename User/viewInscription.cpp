#include "viewInscription.h"
#include "ui_viewInscription.h"

ViewInscription::ViewInscription(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::viewInscription)
{
    ui->setupUi(this);
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
    ui->lbl_info->setText("");

    // Verify the fields
    if(ui->ldt_userName->text().isEmpty() || ui->ldt_password->text().isEmpty() || ui->ldt_passwordConf->text().isEmpty() ||
       ui->ldt_password->text().isEmpty() || ui->ldt_passwordConf->text().isEmpty()) {
        // Afficher un avertissement
        ui->lbl_info->setText("<font color='red'>Veuillez mentionnez tous les champs requis.</font>");
    }
    else if(ui->ldt_password->text() != ui->ldt_passwordConf->text())
        // Afficher un avertissement
        ui->lbl_info->setText("<font color='red'>Le mot de passe ne correspond pas.</font>");
    else
    {
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



    }


}




void ViewInscription::closeEvent(QCloseEvent *)
{
    // Enable connexion window once you close the inscription window
    //ViewInscription::QMainWindow.setDisabled(false);
}


QString ViewInscription::getFirstName() const
{
    // Récupérer le prénom
    return ui->ldt_firstName->text();
}

QString ViewInscription::getLastName() const
{
    // Récupérer le nom
    return ui->ldt_lastName->text();
}

QString ViewInscription::getUserName() const
{
    // Récupérer le nom d'utilisateur
    return ui->ldt_userName->text();
}




