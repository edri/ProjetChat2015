#include "viewChat.h"
#include "ui_viewChat.h"
#include <ctime>

viewChat::viewChat(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::viewChat)
{
    ui->setupUi(this);
    ui->ldt_message->setFocus();
    //ui->tbl_messages->header()->close();
}

viewChat::~viewChat()
{
    delete ui;
}

void viewChat::on_btn_send_clicked()
{
    if (!ui->ldt_message->text().trimmed().isEmpty())
    {
        time_t t = time(0);
        struct tm * now = localtime( & t );

        QTreeWidgetItem *item1 = new QTreeWidgetItem(ui->tre_messages);
        item1->setText(0, "[" + QString::number(now->tm_hour) + ":" + QString::number(now->tm_min) + "] <moi>");
        item1->setText(1, ui->ldt_message->text());
        //item1->setFlags(Qt::);

        ui->ldt_message->clear();
    }
}

void viewChat::on_ldt_message_returnPressed()
{
    ui->btn_send->click();
}
