/********************************************************************************
** Form generated from reading UI file 'viewInscription.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEWINSCRIPTION_H
#define UI_VIEWINSCRIPTION_H

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
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_viewInscription
{
public:
    QWidget *centralwidget;
    QLineEdit *ldt_lastName;
    QLineEdit *ldt_profilPicture;
    QToolButton *btn_path;
    QLabel *lbl_passwordConf;
    QLabel *lbl_profilPicture;
    QLabel *lbl_password;
    QLabel *lbl_firstName;
    QPushButton *btn_inscription;
    QLineEdit *ldt_firstName;
    QLineEdit *ldt_password;
    QLineEdit *ldt_passwordConf;
    QPushButton *btn_question;
    QLabel *lbl_mendatory;
    QLabel *lbl_userName;
    QLineEdit *ldt_userName;
    QLabel *lbl_lastName;
    QLabel *lbl_info;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *viewInscription)
    {
        if (viewInscription->objectName().isEmpty())
            viewInscription->setObjectName(QStringLiteral("viewInscription"));
        viewInscription->resize(449, 553);
        centralwidget = new QWidget(viewInscription);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        ldt_lastName = new QLineEdit(centralwidget);
        ldt_lastName->setObjectName(QStringLiteral("ldt_lastName"));
        ldt_lastName->setGeometry(QRect(20, 110, 400, 30));
        QFont font;
        font.setPointSize(11);
        ldt_lastName->setFont(font);
        ldt_profilPicture = new QLineEdit(centralwidget);
        ldt_profilPicture->setObjectName(QStringLiteral("ldt_profilPicture"));
        ldt_profilPicture->setGeometry(QRect(20, 400, 340, 30));
        ldt_profilPicture->setFont(font);
        btn_path = new QToolButton(centralwidget);
        btn_path->setObjectName(QStringLiteral("btn_path"));
        btn_path->setGeometry(QRect(380, 400, 40, 30));
        lbl_passwordConf = new QLabel(centralwidget);
        lbl_passwordConf->setObjectName(QStringLiteral("lbl_passwordConf"));
        lbl_passwordConf->setGeometry(QRect(20, 290, 231, 31));
        lbl_passwordConf->setFont(font);
        lbl_profilPicture = new QLabel(centralwidget);
        lbl_profilPicture->setObjectName(QStringLiteral("lbl_profilPicture"));
        lbl_profilPicture->setGeometry(QRect(20, 370, 231, 31));
        lbl_profilPicture->setFont(font);
        lbl_password = new QLabel(centralwidget);
        lbl_password->setObjectName(QStringLiteral("lbl_password"));
        lbl_password->setGeometry(QRect(20, 230, 121, 21));
        lbl_password->setFont(font);
        lbl_firstName = new QLabel(centralwidget);
        lbl_firstName->setObjectName(QStringLiteral("lbl_firstName"));
        lbl_firstName->setGeometry(QRect(20, 10, 121, 25));
        lbl_firstName->setFont(font);
        btn_inscription = new QPushButton(centralwidget);
        btn_inscription->setObjectName(QStringLiteral("btn_inscription"));
        btn_inscription->setGeometry(QRect(20, 460, 400, 30));
        QFont font1;
        font1.setPointSize(11);
        font1.setBold(true);
        font1.setWeight(75);
        btn_inscription->setFont(font1);
        ldt_firstName = new QLineEdit(centralwidget);
        ldt_firstName->setObjectName(QStringLiteral("ldt_firstName"));
        ldt_firstName->setGeometry(QRect(20, 40, 400, 30));
        ldt_firstName->setFont(font);
        ldt_password = new QLineEdit(centralwidget);
        ldt_password->setObjectName(QStringLiteral("ldt_password"));
        ldt_password->setGeometry(QRect(20, 250, 340, 30));
        ldt_password->setFont(font);
        ldt_password->setEchoMode(QLineEdit::Password);
        ldt_passwordConf = new QLineEdit(centralwidget);
        ldt_passwordConf->setObjectName(QStringLiteral("ldt_passwordConf"));
        ldt_passwordConf->setGeometry(QRect(20, 320, 400, 30));
        ldt_passwordConf->setFont(font);
        ldt_passwordConf->setEchoMode(QLineEdit::Password);
        btn_question = new QPushButton(centralwidget);
        btn_question->setObjectName(QStringLiteral("btn_question"));
        btn_question->setGeometry(QRect(380, 250, 40, 30));
        btn_question->setFont(font);
        lbl_mendatory = new QLabel(centralwidget);
        lbl_mendatory->setObjectName(QStringLiteral("lbl_mendatory"));
        lbl_mendatory->setGeometry(QRect(20, 430, 231, 31));
        QFont font2;
        font2.setPointSize(8);
        lbl_mendatory->setFont(font2);
        lbl_userName = new QLabel(centralwidget);
        lbl_userName->setObjectName(QStringLiteral("lbl_userName"));
        lbl_userName->setGeometry(QRect(20, 150, 121, 21));
        lbl_userName->setFont(font);
        ldt_userName = new QLineEdit(centralwidget);
        ldt_userName->setObjectName(QStringLiteral("ldt_userName"));
        ldt_userName->setGeometry(QRect(20, 180, 400, 30));
        ldt_userName->setFont(font);
        lbl_lastName = new QLabel(centralwidget);
        lbl_lastName->setObjectName(QStringLiteral("lbl_lastName"));
        lbl_lastName->setGeometry(QRect(20, 80, 121, 25));
        lbl_lastName->setFont(font);
        lbl_info = new QLabel(centralwidget);
        lbl_info->setObjectName(QStringLiteral("lbl_info"));
        lbl_info->setGeometry(QRect(30, 500, 391, 16));
        QFont font3;
        font3.setBold(true);
        font3.setWeight(75);
        lbl_info->setFont(font3);
        lbl_info->setAutoFillBackground(false);
        viewInscription->setCentralWidget(centralwidget);
        menubar = new QMenuBar(viewInscription);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 449, 21));
        viewInscription->setMenuBar(menubar);
        statusbar = new QStatusBar(viewInscription);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        viewInscription->setStatusBar(statusbar);

        retranslateUi(viewInscription);

        QMetaObject::connectSlotsByName(viewInscription);
    } // setupUi

    void retranslateUi(QMainWindow *viewInscription)
    {
        viewInscription->setWindowTitle(QApplication::translate("viewInscription", "MainWindow", 0));
        ldt_lastName->setText(QString());
        ldt_profilPicture->setText(QApplication::translate("viewInscription", "C:\\Users\\User\\Pictures", 0));
        btn_path->setText(QApplication::translate("viewInscription", "...", 0));
        lbl_passwordConf->setText(QApplication::translate("viewInscription", "* Retapez le mot de passe", 0));
        lbl_profilPicture->setText(QApplication::translate("viewInscription", "Image de profil", 0));
        lbl_password->setText(QApplication::translate("viewInscription", "* Mot de passe", 0));
        lbl_firstName->setText(QApplication::translate("viewInscription", "Pr\303\251nom", 0));
        btn_inscription->setText(QApplication::translate("viewInscription", "Inscription", 0));
        ldt_firstName->setText(QString());
        ldt_password->setText(QString());
        ldt_passwordConf->setText(QString());
        btn_question->setText(QApplication::translate("viewInscription", "?", 0));
        lbl_mendatory->setText(QApplication::translate("viewInscription", "* champs obligatoires", 0));
        lbl_userName->setText(QApplication::translate("viewInscription", "* Nom d'utilisateur", 0));
        ldt_userName->setText(QString());
        lbl_lastName->setText(QApplication::translate("viewInscription", "Nom", 0));
        lbl_info->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class viewInscription: public Ui_viewInscription {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWINSCRIPTION_H
