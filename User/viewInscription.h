#ifndef VIEWINSCRIPTION_H
#define VIEWINSCRIPTION_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
//#include <QCloseEvent>

namespace Ui {
class viewInscription;
}

class ViewInscription : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewInscription(QWidget *parent = 0);
    ~ViewInscription();

    QString getFirstName() const;

    QString getLastName() const;

    QString getUserName() const;


private slots:
    void on_btn_path_clicked();

    void on_btn_inscription_clicked();

private:
    Ui::viewInscription *ui;
    void closeEvent(QCloseEvent *);
    bool verifyProfileImage();
    const QString passwordRequirement = "<p>Votre mot de passe doit contenir au moins 8 caractère et:</p> <p>une miniscule, une majuscule, un chiffre et un caractère spécial.</p>";
};

#endif // VIEWINSCRIPTION_H
