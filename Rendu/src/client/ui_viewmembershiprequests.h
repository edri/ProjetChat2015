/********************************************************************************
** Form generated from reading UI file 'viewmembershiprequests.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEWMEMBERSHIPREQUESTS_H
#define UI_VIEWMEMBERSHIPREQUESTS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ViewMembershipRequests
{
public:
    QVBoxLayout *verticalLayout;
    QTreeWidget *tre_requests;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_accept;
    QPushButton *btn_refuse;

    void setupUi(QDialog *ViewMembershipRequests)
    {
        if (ViewMembershipRequests->objectName().isEmpty())
            ViewMembershipRequests->setObjectName(QStringLiteral("ViewMembershipRequests"));
        ViewMembershipRequests->resize(400, 300);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/img/chator.png"), QSize(), QIcon::Normal, QIcon::Off);
        ViewMembershipRequests->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(ViewMembershipRequests);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tre_requests = new QTreeWidget(ViewMembershipRequests);
        tre_requests->setObjectName(QStringLiteral("tre_requests"));

        verticalLayout->addWidget(tre_requests);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        btn_accept = new QPushButton(ViewMembershipRequests);
        btn_accept->setObjectName(QStringLiteral("btn_accept"));
        btn_accept->setEnabled(false);

        horizontalLayout->addWidget(btn_accept);

        btn_refuse = new QPushButton(ViewMembershipRequests);
        btn_refuse->setObjectName(QStringLiteral("btn_refuse"));
        btn_refuse->setEnabled(false);

        horizontalLayout->addWidget(btn_refuse);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(ViewMembershipRequests);

        QMetaObject::connectSlotsByName(ViewMembershipRequests);
    } // setupUi

    void retranslateUi(QDialog *ViewMembershipRequests)
    {
        ViewMembershipRequests->setWindowTitle(QApplication::translate("ViewMembershipRequests", "Demandes d'adh\303\251sion", 0));
        QTreeWidgetItem *___qtreewidgetitem = tre_requests->headerItem();
        ___qtreewidgetitem->setText(2, QApplication::translate("ViewMembershipRequests", "Salle", 0));
        ___qtreewidgetitem->setText(1, QApplication::translate("ViewMembershipRequests", "Utilisateur", 0));
        ___qtreewidgetitem->setText(0, QApplication::translate("ViewMembershipRequests", "ID", 0));
        btn_accept->setText(QApplication::translate("ViewMembershipRequests", "Accepter", 0));
        btn_refuse->setText(QApplication::translate("ViewMembershipRequests", "Refuser", 0));
    } // retranslateUi

};

namespace Ui {
    class ViewMembershipRequests: public Ui_ViewMembershipRequests {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWMEMBERSHIPREQUESTS_H
