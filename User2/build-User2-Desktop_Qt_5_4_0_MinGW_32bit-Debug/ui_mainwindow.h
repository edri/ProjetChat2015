/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLineEdit *ldt_firstName;
    QLabel *lbl_firstName;
    QLabel *lbl_lastName;
    QLineEdit *ldt_lastName;
    QLabel *lbl_userName;
    QLabel *lbl_password;
    QLabel *lbl_passwordConf;
    QLabel *lbl_profilPicture;
    QLineEdit *ldt_userName;
    QLineEdit *ldt_password_3;
    QLineEdit *ldt_passwordConf;
    QLineEdit *ldt_profilPicture;
    QPushButton *btn_inscription;
    QLabel *lbl_mendatory;
    QToolButton *btn_path;
    QPushButton *btn_question;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(440, 600);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        ldt_firstName = new QLineEdit(centralWidget);
        ldt_firstName->setObjectName(QStringLiteral("ldt_firstName"));
        ldt_firstName->setGeometry(QRect(20, 40, 400, 30));
        QFont font;
        font.setPointSize(11);
        ldt_firstName->setFont(font);
        lbl_firstName = new QLabel(centralWidget);
        lbl_firstName->setObjectName(QStringLiteral("lbl_firstName"));
        lbl_firstName->setGeometry(QRect(20, 10, 121, 25));
        lbl_firstName->setFont(font);
        lbl_lastName = new QLabel(centralWidget);
        lbl_lastName->setObjectName(QStringLiteral("lbl_lastName"));
        lbl_lastName->setGeometry(QRect(20, 80, 121, 25));
        lbl_lastName->setFont(font);
        ldt_lastName = new QLineEdit(centralWidget);
        ldt_lastName->setObjectName(QStringLiteral("ldt_lastName"));
        ldt_lastName->setGeometry(QRect(20, 110, 400, 30));
        ldt_lastName->setFont(font);
        lbl_userName = new QLabel(centralWidget);
        lbl_userName->setObjectName(QStringLiteral("lbl_userName"));
        lbl_userName->setGeometry(QRect(20, 150, 121, 21));
        lbl_userName->setFont(font);
        lbl_password = new QLabel(centralWidget);
        lbl_password->setObjectName(QStringLiteral("lbl_password"));
        lbl_password->setGeometry(QRect(20, 230, 121, 21));
        lbl_password->setFont(font);
        lbl_passwordConf = new QLabel(centralWidget);
        lbl_passwordConf->setObjectName(QStringLiteral("lbl_passwordConf"));
        lbl_passwordConf->setGeometry(QRect(20, 290, 231, 31));
        lbl_passwordConf->setFont(font);
        lbl_profilPicture = new QLabel(centralWidget);
        lbl_profilPicture->setObjectName(QStringLiteral("lbl_profilPicture"));
        lbl_profilPicture->setGeometry(QRect(20, 370, 231, 31));
        lbl_profilPicture->setFont(font);
        ldt_userName = new QLineEdit(centralWidget);
        ldt_userName->setObjectName(QStringLiteral("ldt_userName"));
        ldt_userName->setGeometry(QRect(20, 180, 400, 30));
        ldt_userName->setFont(font);
        ldt_password_3 = new QLineEdit(centralWidget);
        ldt_password_3->setObjectName(QStringLiteral("ldt_password_3"));
        ldt_password_3->setGeometry(QRect(20, 250, 340, 30));
        ldt_password_3->setFont(font);
        ldt_passwordConf = new QLineEdit(centralWidget);
        ldt_passwordConf->setObjectName(QStringLiteral("ldt_passwordConf"));
        ldt_passwordConf->setGeometry(QRect(20, 320, 400, 30));
        ldt_passwordConf->setFont(font);
        ldt_profilPicture = new QLineEdit(centralWidget);
        ldt_profilPicture->setObjectName(QStringLiteral("ldt_profilPicture"));
        ldt_profilPicture->setGeometry(QRect(20, 400, 340, 30));
        ldt_profilPicture->setFont(font);
        btn_inscription = new QPushButton(centralWidget);
        btn_inscription->setObjectName(QStringLiteral("btn_inscription"));
        btn_inscription->setGeometry(QRect(20, 490, 400, 30));
        QFont font1;
        font1.setPointSize(11);
        font1.setBold(true);
        font1.setWeight(75);
        btn_inscription->setFont(font1);
        lbl_mendatory = new QLabel(centralWidget);
        lbl_mendatory->setObjectName(QStringLiteral("lbl_mendatory"));
        lbl_mendatory->setGeometry(QRect(20, 450, 231, 31));
        QFont font2;
        font2.setPointSize(8);
        lbl_mendatory->setFont(font2);
        btn_path = new QToolButton(centralWidget);
        btn_path->setObjectName(QStringLiteral("btn_path"));
        btn_path->setGeometry(QRect(380, 400, 40, 30));
        btn_question = new QPushButton(centralWidget);
        btn_question->setObjectName(QStringLiteral("btn_question"));
        btn_question->setGeometry(QRect(380, 250, 40, 30));
        btn_question->setFont(font);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 440, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        ldt_firstName->setText(QString());
        lbl_firstName->setText(QApplication::translate("MainWindow", "Pr\303\251nom", 0));
        lbl_lastName->setText(QApplication::translate("MainWindow", "Nom", 0));
        ldt_lastName->setText(QString());
        lbl_userName->setText(QApplication::translate("MainWindow", "* Nom d'utilisateur", 0));
        lbl_password->setText(QApplication::translate("MainWindow", "* Mot de passe", 0));
        lbl_passwordConf->setText(QApplication::translate("MainWindow", "* Retapez le mot de passe", 0));
        lbl_profilPicture->setText(QApplication::translate("MainWindow", "Image de profil", 0));
        ldt_userName->setText(QString());
        ldt_password_3->setText(QString());
        ldt_passwordConf->setText(QString());
        ldt_profilPicture->setText(QApplication::translate("MainWindow", "C:\\Users\\User\\Pictures", 0));
        btn_inscription->setText(QApplication::translate("MainWindow", "Inscription", 0));
        lbl_mendatory->setText(QApplication::translate("MainWindow", "* champs obligatoires", 0));
        btn_path->setText(QApplication::translate("MainWindow", "...", 0));
        btn_question->setText(QApplication::translate("MainWindow", "?", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
