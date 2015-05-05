#ifndef VIEWCHAT_H
#define VIEWCHAT_H

#include <QMainWindow>
#include <QDate>
#include <QBitmap>

namespace Ui
{
    class viewChat;
}

class ViewChat : public QMainWindow
{
    Q_OBJECT

private:
    Ui::viewChat* _ui;
    quint32 _selectedRoomId;

public:
    explicit ViewChat(QWidget *parent = 0);
    ~ViewChat();

    void setConnectedAsText(const QString& user);
    void addRoom(const quint32 roomId, const QString& roomName, const QImage& roomPicture);
    void addUserToRoom(const quint32 roomId, const quint32 userId, const QString& userName, const QImage& image, const bool isConnected);
    void selectFirstRoom() const;
    void loadRoomMessage(const quint32 roomId, const quint32 messageId, const QString& userName, const QString& content, const QDateTime& date);

    QString getMessageText() const;
    quint32 getSelectedRoomId() const;

private slots:
    void on_btn_send_clicked();
    void on_ldt_message_returnPressed();
    void on_btn_newRoom_clicked();
    void on_btn_joinRoom_clicked();
    void on_btn_leaveRoom_clicked();
    void on_tre_rooms_itemSelectionChanged();

signals:
    void requestLoadRoomMessages(const quint32 roomId) const;
    void requestOpenRoomModule() const;
    void requestSendMessage() const;
};

#endif // VIEWCHAT_H
