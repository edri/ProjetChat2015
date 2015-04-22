#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_inscription_clicked()
{
    // Envoyer les données au serveur
}

void MainWindow::on_btn_path_clicked()
{
    // Chercher l'image à charger

    QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString(), "Images (*.png *.gif *.jpg *.jpeg)");
   // QMessageBox::information(this, "Fichier", "Vous avez sélectionné :\n" + fichier);

}
