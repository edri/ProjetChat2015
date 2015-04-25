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

}

//void ViewInscription::closeEvent(QCloseEvent *)
//{

//}
