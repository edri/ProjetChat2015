#include "viewprofileedition.h"
#include "ui_viewprofileedition.h"

ViewProfilEedition::ViewProfilEedition(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewProfilEedition)
{
    ui->setupUi(this);
}

ViewProfilEedition::~ViewProfilEedition()
{
    delete ui;
}

void ViewProfilEedition::on_btn_modifier_clicked()
{
   //Vérifier les champs avec la même fonction que pour inscription ?
    /*
     * if(verifyFields())
     * {
     *      emit requestModifiyUser();
     *
     * }
     */
}

QString ViewProfilEedition::getFirstName() const
{
    return ui->ldt_firstName->text();
}

QString ViewProfilEedition::getLastName() const
{
    return ui->ldt_lastName->text();
}

QString ViewProfilEedition::getUserName() const
{
    //return ui->ldt_userName->text();
    return "";
}

QString ViewProfilEedition::getPassword() const
{
    return ui->ldt_password->text();
}

QImage ViewProfilEedition::getProfileImage() const
{
    return QImage(ui->ldt_profilPicture->text());
}

bool ViewProfilEedition::isTimeShowed() const
{
    return ui->chk_showTime->isChecked();
}
