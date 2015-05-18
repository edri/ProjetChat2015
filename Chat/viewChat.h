#ifndef VIEWCHAT_H
#define VIEWCHAT_H

#include "../ModeleChator/modelChator.h"
#include <QMainWindow>
#include <QBitmap>
#include <QDate>
#include <QMap>
#include <QMessageBox>
#include <QTreeWidgetItem>

namespace Ui
{
    class viewChat;
}

enum class NotificationType : quint32 {NEW_MEMBERSHIP_APPLICATION, NBR_ITEMS};

class ViewChat : public QMainWindow
{
    Q_OBJECT

private:
    Ui::viewChat* _ui;
    ModelChator* _model;
    quint32 _selectedRoomId;
    bool _isEditingMessage;
    QMenu* _menu;
    quint32 _nbTotalNotifications;
    QList<quint32> _nbNotifications;

    void addMessageToTree(quint32& nbTopMessageItems, ModelMessage& message, const bool isCurrentUserMessage) const;

public:
    static quint32 currentUserId;

    explicit ViewChat(ModelChator* model, QWidget *parent = 0);
    ~ViewChat();

    void resizeEvent(QResizeEvent* event);
    void setConnectedAsText(const QString& user);
    void addRoom(const quint32 roomId, const QString& roomName, const QImage& roomPicture);
    void addUserToRoom(const quint32 roomId, const quint32 userId, const QString& userName, const QImage& image, const bool isConnected);
    void selectFirstRoom() const;
    void loadRoomMessages(const QMap<quint32, ModelMessage>& messages);
    void loadRoomMessage(ModelMessage& message, const bool edited = false);
    QString getMessageText() const;
    quint32 getSelectedRoomId() const;
    void userStatusChanged(const quint32 userId, const bool isConnected) const;
    void updateButtons(const bool isAdmin) const;
    void deleteMessage(const quint32 messageId) const;
    void deleteRoom(const quint32 roomId) const;
    void newNotification(const NotificationType notifType) const;

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
    void showContextMessage(const QPoint& pos);

    void on_btn_expandAll_clicked();

    void on_btn_collapseAll_clicked();

signals:
    void requestLoadRoomMessages(const quint32 roomId) const;
    void requestOpenRoomModule() const;
    void requestSendMessage() const;
    void requestEditMessage(const QTreeWidgetItem* item) const;
    void requestDeleteMessage(const quint32 messageId) const;
    void requestDeleteRoom(const quint32 roomId) const;
    void requestLeaveRoom(const quint32 roomId) const;
};

#endif // VIEWCHAT_H
