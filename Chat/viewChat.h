#ifndef VIEWCHAT_H
#define VIEWCHAT_H

#include <QMainWindow>
#include <QBitmap>
#include <QDate>
#include <QMessageBox>
#include <QTreeWidgetItem>

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
    bool _isEditingMessage;

public:
    explicit ViewChat(QWidget *parent = 0);
    ~ViewChat();

    void setConnectedAsText(const QString& user);
    void addRoom(const quint32 roomId, const QString& roomName, const QImage& roomPicture);
    void addUserToRoom(const quint32 roomId, const quint32 userId, const QString& userName, const QImage& image, const bool isConnected);
    void selectFirstRoom() const;
    void loadRoomMessage(const quint32 roomId, const quint32 messageId, const QString& userName,
                         const QString& content, const QDateTime& date, const QDateTime& lastUpdateDate, const bool isCurrentUsersMessage,
                         const bool edited = false);

    QString getMessageText() const;
    quint32 getSelectedRoomId() const;
    void userStatusChanged(const quint32 userId, const bool isConnected) const;
    void updateButtons(const bool isAdmin) const;
    void deleteRoom(const quint32 roomId) const;

private slots:
    void on_btn_send_clicked();
    void on_ldt_message_returnPressed();
    void on_btn_newRoom_clicked();
    void on_btn_joinRoom_clicked();
    void on_btn_leaveRoom_clicked();
    void on_tre_rooms_itemSelectionChanged();
    void on_tre_messages_itemChanged(QTreeWidgetItem* item, int column);
    void on_tre_messages_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_actionQuitter_triggered();
    void on_btn_edit_clicked();
    void on_btn_delete_clicked();

signals:
    void requestLoadRoomMessages(const quint32 roomId) const;
    void requestOpenRoomModule() const;
    void requestSendMessage() const;
    void requestEditMessage(const QTreeWidgetItem* item) const;
    void requestDeleteRoom(const quint32 roomId) const;
    void requestLeaveRoom(const quint32 roomId) const;
};

#endif // VIEWCHAT_H
