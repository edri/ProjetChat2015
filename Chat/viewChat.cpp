#include "viewChat.h"
#include "ui_viewChat.h"
#include <ctime>

ViewChat::ViewChat(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::viewChat)
{
    ui->setupUi(this);

    ui->tre_messages->expandAll();
    ui->tre_messages->resizeColumnToContents(0);
    ui->ldt_message->setFocus();
}

ViewChat::~ViewChat()
{
    delete ui;
}

void ViewChat::setConnectedAsText(const QString& user)
{
    ui->lbl_connectedAs->setText("Connecté en tant que <b>" + user + "</b>.");
}

void ViewChat::addRoom(const quint32 roomId, const QString& roomName, const QString& roomPicture)
{
    QTreeWidgetItem* roomItem = new QTreeWidgetItem(ui->tre_rooms);
    roomItem->setData(0, Qt::UserRole, roomId);
    roomItem->setText(0, roomName);
    roomItem->setIcon(0, QIcon("img/" + roomPicture));
}

void ViewChat::addUserToRoom(const quint32 roomId, const quint32 userId, const QString &userName, const QString& image, const bool isConnected)
{
    for (int i = 0; i < ui->tre_rooms->topLevelItemCount(); ++i)
    {
        if (ui->tre_rooms->topLevelItem(i)->data(0, Qt::UserRole).toInt() == roomId)
        {
            QTreeWidgetItem* userItem = new QTreeWidgetItem(ui->tre_rooms->topLevelItem(i));
            userItem->setData(0, Qt::UserRole, userId);
            userItem->setText(0, userName);
            userItem->setIcon(0, QIcon("img/" + image));

            if (isConnected)
                userItem->setFont(0, QFont("MS Shell Dlg 2", 8, QFont::Bold));

            break;
        }
    }
}

void ViewChat::selectFirstRoom() const
{
    if (ui->tre_rooms->topLevelItemCount())
        ui->tre_rooms->setCurrentItem(ui->tre_rooms->topLevelItem(0));
}

void ViewChat::on_btn_send_clicked()
{
    if (!ui->ldt_message->text().trimmed().isEmpty())
    {
        /*time_t t = time(0);
        struct tm * now = localtime( & t );

        QTreeWidgetItem *item1 = new QTreeWidgetItem(ui->tre_messages);
        item1->setText(0, "[" + QString::number(now->tm_hour) + ":" + QString::number(now->tm_min) + "] <moi>");
        item1->setText(1, ui->ldt_message->text());
        //item1->setFlags(Qt::);*/

        ui->ldt_message->clear();
    }
}

void ViewChat::on_ldt_message_returnPressed()
{
    ui->btn_send->click();
}

void ViewChat::on_btn_leaveRoom_clicked()
{
    quint32 d = ui->tre_rooms->selectedItems().at(0)->data(0, Qt::UserRole).toInt();
    ui->ldt_message->setText(QString::number(d));
}

void ViewChat::on_btn_joinRoom_clicked()
{

}

void ViewChat::on_btn_newRoom_clicked()
{

}

void ViewChat::on_tre_rooms_itemSelectionChanged()
{
    for (int i = 0; i < ui->tre_rooms->topLevelItemCount(); ++i)
    {
        if (ui->tre_rooms->topLevelItem(i)->isSelected())
        {
            ui->tre_rooms->topLevelItem(i)->setFont(0, QFont("MS Shell Dlg 2", 9, QFont::Bold));
            ui->tre_rooms->expandItem(ui->tre_rooms->topLevelItem(i));
        }
        else
            ui->tre_rooms->topLevelItem(i)->setFont(0, QFont("MS Shell Dlg 2", 9, QFont::Normal));
    }
}
