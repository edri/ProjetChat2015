#ifndef VIEWCHAT_H
#define VIEWCHAT_H

#include <QMainWindow>

namespace Ui {
class viewChat;
}

class viewChat : public QMainWindow
{
    Q_OBJECT

public:
    explicit viewChat(QWidget *parent = 0);
    ~viewChat();

private slots:
    void on_btn_send_clicked();

    void on_ldt_message_returnPressed();

private:
    Ui::viewChat *ui;
};

#endif // VIEWCHAT_H
