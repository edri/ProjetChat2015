#ifndef VIEWUSER_H
#define VIEWUSER_H

#include <QMainWindow>

namespace Ui {
class ViewUser;
}

class ViewUser : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewUser(QWidget *parent = 0);
    ~ViewUser();

    QString getUsername();
    QString getPassword();
    QString getPort();
    QString getIpAddress();

private slots:      // Fonctions appelées lorsqu'un événement se produit (signal = msg envoyé par un objet)

    // Envoyer le nom d'utilisateur et le mot de passe au serveur?
    // Le serveur vérifie s'ils sont valides?
    void on_btn_connexion_clicked();

    // Ouvrir la fenêtre inscription!
    void on_btn_inscription_clicked();

private:
    Ui::ViewUser *ui;


signals:
    // Demander à récupérer les identifiants
    void requestGetIds() const;
};

#endif // VIEWUSER_H
