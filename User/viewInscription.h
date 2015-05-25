#ifndef VIEWINSCRIPTION_H
#define VIEWINSCRIPTION_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include "../ModeleChator/modelChator.h"
//#include <QCloseEvent>

namespace Ui {
class viewInscription;
}

class ViewInscription : public QMainWindow
{
    Q_OBJECT

public:
    ViewInscription(QWidget *parent = 0);
    ViewInscription(QWidget *parent, ModelUser* currentUser);

    ~ViewInscription();

    QString getFirstName() const;
    QString getLastName() const;
    QString getUserName() const;
    QString getPassword() const;
    QImage getProfileImage() const;
    ModelUser* getCurrentUser();
    void setCurrentUser(ModelUser* currentUser);

private slots:
    void on_btn_path_clicked();

    void on_btn_inscription_clicked();

private:
    Ui::viewInscription *ui;
    void closeEvent(QCloseEvent *);
    bool verifyFields();
    bool verifyProfileImage();
    QString passwordRequirement;
    bool edition;
    ModelUser* currentUser;

signals:
    // Ask to get the information of the user inscription
    void requestGetNewUser() const;

    void requestEditUser() const;
};

#endif // VIEWINSCRIPTION_H
