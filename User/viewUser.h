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
    void requestGetIds() const;

    // Ask to get the information of the user inscription
    void requestGetNewUser() const;
};

#endif // VIEWUSER_H
