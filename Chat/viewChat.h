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

    void setConnectedAsText(const QString& user);
    void addRoom(const quint32 roomId, const QString& roomName, const QString& roomPicture);
    void addUserToRoom(const quint32 roomId, const quint32 userId, const QString& userName, const QString& image, const bool isConnected);
    void selectFirstRoom() const;

private slots:
    void on_btn_send_clicked();
    void on_ldt_message_returnPressed();
    void on_btn_newRoom_clicked();
    void on_btn_joinRoom_clicked();
    void on_btn_leaveRoom_clicked();
    void on_tre_rooms_itemSelectionChanged();

private:
    Ui::viewChat *ui;
};

#endif // VIEWCHAT_H
