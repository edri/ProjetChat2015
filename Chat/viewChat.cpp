#include "viewChat.h"
#include "ui_viewChat.h"
#include <ctime>

ViewChat::ViewChat(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::viewChat)
{
    ui->setupUi(this);

    addRoom();

    ui->ldt_message->setFocus();
    //ui->tbl_messages->header()->close();
}

ViewChat::~ViewChat()
{
    delete ui;
}

void ViewChat::addRoom()
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->tre_rooms);
    item->setText(0, "blablabla");
    item->setData(0, 0, 32);
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
    QString d = ui->tre_rooms->selectedItems().at(0)->text(0);
    ui->ldt_message->setText(d);
}

void ViewChat::on_btn_joinRoom_clicked()
{

}

void ViewChat::on_btn_newRoom_clicked()
{

}
