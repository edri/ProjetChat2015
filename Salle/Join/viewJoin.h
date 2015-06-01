/*
 * File : viewJoin.h
 * Project : ProjetChat2015
 * Description : Interface used in order for the user to join a room.
 * Will display the public and visible private rooms the user is not a member
 * of.
 * A text field is available for him to filter the displayed rooms.
 * 
 * The view is destroyed when closed.
*/

#ifndef CHAT_JOIN_H
#define CHAT_JOIN_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QListView>
#include <QStandardItemModel>

class ControllerRoom;

class ViewJoin : public QWidget
{
    Q_OBJECT
    
public:

    // Constructor
    ViewJoin(); 
    // Destructor
    ~ViewJoin();
    
    // Setters for the view's list of available rooms to user.
    void setPublicRooms(const QList<QPair<quint32, QString>>& publicRooms);
    void setPrivateRooms(const QList<QPair<quint32, QString>>& privateRooms);
    
    // Load all the available rooms in the view. The private rooms will be displayed
    // in a bold font and the public rooms in a normal font.
    void loadRooms();
    
public slots:
    
    // Will load the available rooms containing the subtring in the view. The private 
    // rooms will be displayed in a bold font and the public rooms in a normal font.
    // substring : the substring the room's name are serached for.
    void filterRooms(const QString& substring);
    
    // Retrieve the selected room from the view, if there's a selected room, returns
    // doing nothing otherwise.
    void checkRoom();

signals:
    // Signal emited when the user wishes to cancel the operation.
    void cancel();
    // Signal emited when the user wishes to join a room.
    // roomId : the room's id.
    // isPrivate : wether the room is private or not.
    void join(quint32 roomId, bool isPrivate);
    // Signal emited when the user closed the view.
    void reactivateChatWindows();

private:
    
    // Core elements
    QList<QLayout*>* layouts;
    QList<QPair<quint32, QString>> _publicRooms;
    QList<QPair<quint32, QString>> _privateRooms;
    
    // GUI elements
    QLabel* label_title;
    QLabel* label_name;
    QLabel* label_rooms;
    
    QLineEdit* ledit_name;
    
    QPushButton* btn_join;
    QPushButton* btn_cancel;
    
    QListView* list_rooms;
    
    QStandardItemModel* model_rooms;

    // Function called when the view is closed
    void closeEvent(QCloseEvent*);
};

#endif //CHAT_JOIN_H
