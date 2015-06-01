/********************************************************************************
** Form generated from reading UI file 'viewabout.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEWABOUT_H
#define UI_VIEWABOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_viewAbout
{
public:
    QLabel *lbl_title;
    QLabel *lbl_lastCompilation;
    QLabel *lbl_copyright;
    QGraphicsView *grv_logo;
    QGraphicsView *grv_heig;

    void setupUi(QDialog *viewAbout)
    {
        if (viewAbout->objectName().isEmpty())
            viewAbout->setObjectName(QStringLiteral("viewAbout"));
        viewAbout->resize(405, 127);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/img/chator.png"), QSize(), QIcon::Normal, QIcon::Off);
        viewAbout->setWindowIcon(icon);
        lbl_title = new QLabel(viewAbout);
        lbl_title->setObjectName(QStringLiteral("lbl_title"));
        lbl_title->setGeometry(QRect(130, 10, 131, 20));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lbl_title->sizePolicy().hasHeightForWidth());
        lbl_title->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        lbl_title->setFont(font);
        lbl_lastCompilation = new QLabel(viewAbout);
        lbl_lastCompilation->setObjectName(QStringLiteral("lbl_lastCompilation"));
        lbl_lastCompilation->setGeometry(QRect(130, 40, 261, 16));
        lbl_copyright = new QLabel(viewAbout);
        lbl_copyright->setObjectName(QStringLiteral("lbl_copyright"));
        lbl_copyright->setGeometry(QRect(130, 60, 271, 16));
        grv_logo = new QGraphicsView(viewAbout);
        grv_logo->setObjectName(QStringLiteral("grv_logo"));
        grv_logo->setGeometry(QRect(10, 10, 107, 107));
        grv_logo->setStyleSheet(QStringLiteral("background: transparent;"));
        grv_logo->setFrameShape(QFrame::NoFrame);
        grv_heig = new QGraphicsView(viewAbout);
        grv_heig->setObjectName(QStringLiteral("grv_heig"));
        grv_heig->setGeometry(QRect(130, 86, 91, 31));
        grv_heig->setStyleSheet(QStringLiteral("background: transparent;"));
        grv_heig->setFrameShape(QFrame::NoFrame);

        retranslateUi(viewAbout);

        QMetaObject::connectSlotsByName(viewAbout);
    } // setupUi

    void retranslateUi(QDialog *viewAbout)
    {
        viewAbout->setWindowTitle(QApplication::translate("viewAbout", "\303\200 propos de Chator", 0));
        lbl_title->setText(QApplication::translate("viewAbout", "Chator 1.1.2", 0));
        lbl_lastCompilation->setText(QApplication::translate("viewAbout", "Derni\303\250re compilation effectu\303\251e le 02.06.2015", 0));
        lbl_copyright->setText(QApplication::translate("viewAbout", "Copyright 2015 HEIG-VD. Tous droits r\303\251serv\303\251s.", 0));
    } // retranslateUi

};

namespace Ui {
    class viewAbout: public Ui_viewAbout {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWABOUT_H
