/*
     * Created by Mélanie Huck
     * Edited by Bastien Rouiller
     *
     * Inform controllerUser when the user finish completing
     * the connection forms or want to create a new account.
     *
*/

#ifndef VIEWUSER_H
#define VIEWUSER_H

#include <QMainWindow>
#include "viewInscription.h"

namespace Ui {
class ViewUser;
}

class ViewUser : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewUser(QWidget *parent = 0);
    ~ViewUser();

    QString getUsername() const;
    QString getPassword() const;
    QString getPort() const;
    QString getIpAddress() const;
    ViewInscription* getViewInscription() const;

private slots:      // Fonctions appelées lorsqu'un événement se produit (signal = msg envoyé par un objet)

    // Envoyer le nom d'utilisateur et le mot de passe au serveur?
    // Le serveur vérifie s'ils sont valides?
    void on_btn_connexion_clicked();

    // Ouvrir la fenêtre inscription!
    void on_btn_inscription_clicked();

private:
    Ui::ViewUser* ui;
    ViewInscription* _viewInscription;


signals:
    // Ask to get the login
    void requestGetIds(bool fromBtnConnection) const;
};

#endif // VIEWUSER_H
