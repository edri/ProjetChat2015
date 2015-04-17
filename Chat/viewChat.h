#ifndef VIEWCHAT_H
#define VIEWCHAT_H

#include <QMainWindow>

namespace Ui {
class viewChat;
}

class ControllerChat;

class ViewChat : public QMainWindow
{
    Q_OBJECT

    friend ControllerChat;

public:
    explicit ViewChat(QWidget *parent = 0);
    ~ViewChat();

    void addRoom();

private slots:
    void on_btn_send_clicked();
    void on_ldt_message_returnPressed();
    void on_btn_newRoom_clicked();
    void on_btn_joinRoom_clicked();
    void on_btn_leaveRoom_clicked();

private:
    Ui::viewChat *ui;
};

#endif // VIEWCHAT_H
