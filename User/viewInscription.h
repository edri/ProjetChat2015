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
    QString getPassword() const;
    QImage getProfileImage() const;

private slots:
    void on_btn_path_clicked();

    void on_btn_inscription_clicked();

private:
    Ui::viewInscription *ui;
    void closeEvent(QCloseEvent *);
    bool verifyFields();
    bool verifyProfileImage();
    QString passwordRequirement;

signals:
    // Ask to get the information of the user inscription
    void requestGetNewUser() const;



};

#endif // VIEWINSCRIPTION_H
