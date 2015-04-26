/********************************************************************************
** Form generated from reading UI file 'viewUser.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEWUSER_H
#define UI_VIEWUSER_H

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
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ViewUser
{
public:
    QWidget *centralWidget;
    QPushButton *btn_inscription;
    QLabel *label_3;
    QLabel *label;
    QLabel *label_4;
    QLabel *label_2;
    QPushButton *btn_connexion;
    QLineEdit *ldt_Username;
    QLineEdit *ldt_server;
    QLineEdit *ldt_password;
    QSpinBox *spinBox;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ViewUser)
    {
        if (ViewUser->objectName().isEmpty())
            ViewUser->setObjectName(QStringLiteral("ViewUser"));
        ViewUser->resize(500, 342);
        centralWidget = new QWidget(ViewUser);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        btn_inscription = new QPushButton(centralWidget);
        btn_inscription->setObjectName(QStringLiteral("btn_inscription"));
        btn_inscription->setGeometry(QRect(270, 260, 210, 30));
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        btn_inscription->setFont(font);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 170, 200, 25));
        QFont font1;
        font1.setPointSize(11);
        label_3->setFont(font1);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setEnabled(true);
        label->setGeometry(QRect(20, 10, 459, 25));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMaximumSize(QSize(500, 25));
        label->setFont(font1);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(270, 170, 210, 25));
        label_4->setFont(font1);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 90, 459, 25));
        label_2->setMaximumSize(QSize(500, 25));
        label_2->setFont(font1);
        btn_connexion = new QPushButton(centralWidget);
        btn_connexion->setObjectName(QStringLiteral("btn_connexion"));
        btn_connexion->setGeometry(QRect(20, 260, 210, 30));
        btn_connexion->setFont(font);
        ldt_Username = new QLineEdit(centralWidget);
        ldt_Username->setObjectName(QStringLiteral("ldt_Username"));
        ldt_Username->setGeometry(QRect(20, 40, 459, 30));
        ldt_Username->setMaximumSize(QSize(500, 30));
        ldt_Username->setFont(font1);
        ldt_server = new QLineEdit(centralWidget);
        ldt_server->setObjectName(QStringLiteral("ldt_server"));
        ldt_server->setGeometry(QRect(20, 200, 210, 30));
        ldt_server->setFont(font1);
        ldt_password = new QLineEdit(centralWidget);
        ldt_password->setObjectName(QStringLiteral("ldt_password"));
        ldt_password->setGeometry(QRect(20, 120, 459, 30));
        ldt_password->setMaximumSize(QSize(16777215, 30));
        ldt_password->setFont(font1);
        ldt_password->setInputMethodHints(Qt::ImhHiddenText|Qt::ImhNoAutoUppercase|Qt::ImhNoPredictiveText|Qt::ImhSensitiveData);
        ldt_password->setEchoMode(QLineEdit::Password);
        spinBox = new QSpinBox(centralWidget);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setGeometry(QRect(270, 200, 210, 30));
        ViewUser->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ViewUser);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 500, 21));
        ViewUser->setMenuBar(menuBar);
        statusBar = new QStatusBar(ViewUser);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ViewUser->setStatusBar(statusBar);

        retranslateUi(ViewUser);

        QMetaObject::connectSlotsByName(ViewUser);
    } // setupUi

    void retranslateUi(QMainWindow *ViewUser)
    {
        ViewUser->setWindowTitle(QApplication::translate("ViewUser", "ViewUser", 0));
        btn_inscription->setText(QApplication::translate("ViewUser", "Inscription", 0));
        label_3->setText(QApplication::translate("ViewUser", "Serveur", 0));
        label->setText(QApplication::translate("ViewUser", "Nom d'utilisateur", 0));
        label_4->setText(QApplication::translate("ViewUser", "Port", 0));
        label_2->setText(QApplication::translate("ViewUser", "Mot de passe", 0));
        btn_connexion->setText(QApplication::translate("ViewUser", "Connexion", 0));
        ldt_server->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ViewUser: public Ui_ViewUser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWUSER_H
