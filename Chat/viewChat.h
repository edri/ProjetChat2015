/*
     * Created by Miguel Santamaria
     *
     * Show information in the GUI received from the controllerChat:
     * - Messages receives
     * - Available rooms
     * - Members in the room
*/

#ifndef VIEWCHAT_H
#define VIEWCHAT_H

#include "../ModeleChator/modelChator.h"
#include "../User/viewInscription.h"
#include "viewabout.h"
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

class ViewChat : public QMainWindow
{
    Q_OBJECT

private:
    Ui::viewChat* _ui;
    ModelChator* _model;
    quint32 _selectedRoomId;
    bool _isEditingMessage;
    QMenu* _menu;
    quint32 _nbNotifications;
    ViewAbout* _viewAbout;

    void addMessageToTree(quint32& nbTopMessageItems, ModelMessage& message, const bool isCurrentUserMessage) const;

public:
    static quint32 currentUserId;

    explicit ViewChat(ModelChator* model, QWidget *parent = 0);
    ~ViewChat();

    void resizeEvent(QResizeEvent* event);

    //----------------------------------------------------------------------------------
    // Goal      : Set the label "Connected as ..." with the user name
    // Param     : user - username of the current user
    //----------------------------------------------------------------------------------
    void setConnectedAsText(const QString& user);

    //----------------------------------------------------------------------------------
    // Goal      : Add a room on the list of rooms in the chat's window.
    // Param     : roomId - Identify the room that is added to the view.
    //             roomName - Name of the room
    //             roomPicture - Picture of the room
    //             isPrivate - Indicate if the room is private (true) or not (false).
    //----------------------------------------------------------------------------------
    void addRoom(const quint32 roomId, const QString& roomName, const QImage& roomPicture,
                 const bool isPrivate);

    //----------------------------------------------------------------------------------
    // Goal      : Edit a room's data in the chat's window.
    // Param     : roomId - Identify the room that is added to the view.
    //             roomName - Name of the room
    //             roomPicture - Picture of the room
    //----------------------------------------------------------------------------------
    void modifyRoom(const quint32 roomId, const QString& roomName, const QImage& roomPicture);

    //----------------------------------------------------------------------------------
    // Goal      : Add a user in a room, below a room in the list of room.
    // Param     : roomId - Identify the room where the message should be deleted.
    //             userId - Identify the message that has to be deleted
    //             userName - Username of the new user
    //             image - User's profile image
    //             isConnected - Inform if user is connected or not
    //----------------------------------------------------------------------------------
    void addUserToRoom(const quint32 roomId, const quint32 userId, const QString& userName, const QImage& image, const bool isConnected);

    //----------------------------------------------------------------------------------
    // Goal      : unknown
    // Param     : /
    //----------------------------------------------------------------------------------
    void selectFirstRoom() const;

    //----------------------------------------------------------------------------------
    // Goal      : unknown
    // Param     : messages -
    //----------------------------------------------------------------------------------
    void loadRoomMessages(const QMap<quint32, ModelMessage>& messages);

    //----------------------------------------------------------------------------------
    // Goal      : unknown
    // Param     : message -
    //             edited -
    //----------------------------------------------------------------------------------
    void loadRoomMessage(ModelMessage& message, const bool edited = false);

    //----------------------------------------------------------------------------------
    // Goal      : Return the message the user has written (in the specified field).
    // Param     : /
    //----------------------------------------------------------------------------------
    QString getMessageText() const;

    quint32 getSelectedRoomId() const;

    //----------------------------------------------------------------------------------
    // Goal      : Update the list of users. Connected users appeared with bold format.
    // Param     : userId -
    //             isConnected -
    //----------------------------------------------------------------------------------
    void userStatusChanged(const quint32 userId, const bool isConnected) const;

    //----------------------------------------------------------------------------------
    // Goal      : Enable buttons when user is admin: edit and delete button when a room
    //             is selected.
    // Param     : isAdmin - Determine if user is admin
    //----------------------------------------------------------------------------------
    void updateButtons(const bool isAdmin) const;

    //----------------------------------------------------------------------------------
    // Goal      : Called when a message has been deleted, remove the message from the
    //             windows.
    // Param     : message -
    //             edited -
    //----------------------------------------------------------------------------------
    void deleteMessage(const quint32 messageId) const;

    //----------------------------------------------------------------------------------
    // Goal      : Remove a room from the list
    // Param     : roomId - Identify the room where the room should be removed.
    //----------------------------------------------------------------------------------
    void deleteRoom(const quint32 roomId);

    //----------------------------------------------------------------------------------
    // Goal      : Remove a user from a room in the list
    // Param     : userId - Identify the user to remove.
    //             roomId - Identify the room where the user should be removed.
    //----------------------------------------------------------------------------------
    void deleteUserFromRoom(const quint32 userId, const quint32 roomId);

    //----------------------------------------------------------------------------------
    // Goal      : Update the notifications' toolbar and the view.
    // Param     : - nbToUpdate : the number of new notifications (positive number) or
    //                            notifications to clear (negative number).
    //----------------------------------------------------------------------------------
    void updateRequests(const qint32 nbToUpdate);

    //----------------------------------------------------------------------------------
    // Goal      : Display an error message indicating that the server's connexion has
    //             been lost, and notify the controller to close the application.
    // Param     : /
    //----------------------------------------------------------------------------------
    void serverDisconnected();

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
    void on_actionCompte_triggered();
    void on_actionDemandes_d_adh_sion_triggered();
    void on_actionA_propos_triggered();

signals:
    void requestLoadRoomMessages(const quint32 roomId) const;
    void requestOpenRoomModule(const bool editRoom) const;
    void requestSendMessage() const;
    void requestEditMessage(const QTreeWidgetItem* item) const;
    void requestDeleteMessage(const quint32 roomId, const quint32 messageId) const;
    void requestDeleteRoom(const quint32 roomId) const;
    void requestLeaveRoom(const quint32 roomId) const;
    void requestShowEditionView() const;
    void requestOpenRoomMembership() const;
    void requestShowMembershipRequestsView() const;
    void requestCloseApplication() const;
};

#endif // VIEWCHAT_H
