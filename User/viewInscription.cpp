/*
     * Created by Mélanie Huck
     * Edited by Bastien Rouiller
     *
     * Implements viewInscription.h
     *
*/

#include "viewInscription.h"
#include "ui_viewInscription.h"
#include <QDebug>
#include <QStandardPaths>

ViewInscription::ViewInscription(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::viewInscription),
    _edition(false)
{
    _ui->setupUi(this);
    _passwordRequirement = "<p>Votre mot de passe doit contenir au moins 8 caractère et:</p> <p>une miniscule, une majuscule, un chiffre et un caractère spécial.</p>";
}

ViewInscription::ViewInscription(QWidget *parent, ModelUser* currentUser) :
    QMainWindow(parent),
    _ui(new Ui::viewInscription)
{
    //We modify the original View in order to have the edition view
    _edition = true;
    //this->currentUser = currentUser;

    _ui->setupUi(this);

    this->setWindowTitle("Compte");

    _ui->btn_question->setVisible(false);
    _ui->btn_inscription->setVisible(false);

    _ui->lbl_userName->setText("Nom d'utilisateur");
    _ui->ldt_userName->setDisabled(true);
    _ui->ldt_firstName->setDisabled(true);
    _ui->ldt_lastName->setDisabled(true);

    _ui->lbl_password->setVisible(false);
    _ui->ldt_password->setVisible(false);
    _ui->lbl_passwordConf->setVisible(false);
    _ui->ldt_passwordConf->setVisible(false);
    _ui->ldt_profilPicture->setVisible(false);
    _ui->btn_path->setVisible(false);
    //ui->lbl_password->setText("Nouveau mot de passe");
    //ui->lbl_passwordConf->setText("Veuillez retaper le nouveau mot de passe");

    _ui->lbl_mendatory->setText("");
}

ViewInscription::~ViewInscription()
{
    delete _ui;
}

void ViewInscription::usernameAlreadyExistd()
{
    QMessageBox::critical(this, tr("Le nom d'utilisateur existe déjà..."),
                          tr("Le nom d'utilisateur entré existe déjà. Veuillez en entrer un autre."),
                          QMessageBox::Ok);
    setEnabled(true);
    _ui->ldt_userName->setFocus();
}

void ViewInscription::on_btn_path_clicked()
{
    // Browse the files in order to find a picture
    QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QStandardPaths::writableLocation(QStandardPaths::PicturesLocation), "Images (*.png *.gif *.jpg *.jpeg);;Autres (*.*)");

    _ui->ldt_profilPicture->setText(fichier);
    // QMessageBox::information(this, "Fichier", "Vous avez sélectionné :\n" + fichier);
}

void ViewInscription::on_btn_inscription_clicked()
{
    if(!_edition && verifyFields())
    {
        setEnabled(false);
        emit requestGetNewUser();
        _ui->lbl_info->setText("Envoi au serveur");
    }
}

bool ViewInscription::verifyFields()
{
    // Regular expressions
    QRegExp passwordRestriction("^(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])(?=.*[^a-z^A-Z^0-9]).{8,}$");
    QRegExp userNameRestriction("^[a-zA-Z0-9_ -]*$");

    // Verify the fields
    if(_ui->ldt_userName->text().isEmpty() || _ui->ldt_password->text().isEmpty() || _ui->ldt_passwordConf->text().isEmpty() ||
       _ui->ldt_password->text().isEmpty() || _ui->ldt_passwordConf->text().isEmpty())
    {
        _ui->lbl_info->setText("<font color='red'>Veuillez mentionnez tous les champs requis.</font>");
        return false;
    }
    else if(!userNameRestriction.exactMatch(_ui->ldt_userName->text()))
    {
        _ui->lbl_info->setText("<font color='red'>Le nom d'utilisateur doit contenir des chiffres, des lettres ou - et _</font>");
        return false;
    }
    else if(_ui->ldt_userName->text().toLower() == tr("Anonyme").toLower())
    {
        _ui->lbl_info->setText("<font color='red'>Ce nom d'utilisateur est un nom réservé par le système.</font>");
        return false;
    }
    else if(_ui->ldt_password->text() != _ui->ldt_passwordConf->text())
    {
        _ui->lbl_info->setText("<font color='red'>Le mot de passe ne correspond pas.</font>");
        return false;
    }
    else if(_ui->ldt_password->text() == _ui->ldt_userName->text())
    {
        _ui->lbl_info->setText("<font color='red'>Votre mot de passe doit être différent de votre nom d'utilisateur.</font>");
        return false;
    }
    else if(!passwordRestriction.exactMatch(_ui->ldt_password->text()))
    {
        _ui->lbl_info->setText("<font color='red'>" + _passwordRequirement + "</font>");
        return false;
    }
    else if(!_ui->ldt_profilPicture->text().isEmpty() && !verifyProfileImage())
    {
        _ui->lbl_info->setText("<font color='red'>Image spécifiée incorrecte</font>");
        return false;
    }
    else
        return true;
}

bool ViewInscription::verifyProfileImage() const
{
    QImage image(_ui->ldt_profilPicture->text());
    
    return !image.isNull();
}

void ViewInscription::closeEvent(QCloseEvent*)
{
    // clear the info message
    _ui->lbl_info->clear();

    // Enable connexion window once you close the inscription window
    emit requestCancelInscription();
}


QString ViewInscription::getFirstName() const
{
    return _ui->ldt_firstName->text();
}

QString ViewInscription::getLastName() const
{
    return _ui->ldt_lastName->text();
}

QString ViewInscription::getUserName() const
{
    return _ui->ldt_userName->text();
}

QString ViewInscription::getPassword() const
{
    return _ui->ldt_password->text();
}

QImage ViewInscription::getProfileImage() const
{
    if (!verifyProfileImage()){return QImage();}
    
    return QImage(_ui->ldt_profilPicture->text()).scaled(PICTURE_SIZE, PICTURE_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

void ViewInscription::setCurrentUser(ModelUser* currentUser)
{
    //Uptade the view with the information about the user
    this->_currentUser = currentUser;

    _ui->ldt_userName->setText(currentUser->getUserName());
    _ui->ldt_firstName->setText(currentUser->getFirstName());
    _ui->ldt_lastName->setText(currentUser->getLastName());
    _ui->lbl_profilPicture->setPixmap(QPixmap::fromImage(currentUser->getImage()));
}

void ViewInscription::on_btn_cancel_clicked()
{
    close();
}

void ViewInscription::on_btn_question_clicked()
{
    QMessageBox::information(this, tr("Informations sur le mot de passe"),
                             "Votre mot de passe doit contenir au moins 8 caractères, dont une miniscule, une majuscule, un chiffre et un caractère spécial.",
                            QMessageBox::Ok);
}
