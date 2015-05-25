/*
 * File : viewRoom.h
 * Project : ProjetChat2015
 * Author(s) : Jan Purro
 * Last Modified : 25.04.2015 14:25 by Jan Purro
 * Description : View used when creating or editing a room. Part of the room 
 * module.
 * 
 * When the module controller (controllerRoom) construct a view, it will set if 
 * the view is editing or creating a room. The mode can't be changed once the 
 * room is created.
 * 
 * When closed the view is destroyed.
 */

#ifndef CHAT_VIEW_ROOM_H
#define CHAT_VIEW_ROOM_H

#include <QtWidgets>
#include "../../ModeleChator/modelChator.h"

// predeclaration of the controller.
class ControllerRoom;

class ViewRoom : public QWidget
{
    Q_OBJECT
    
    //friend ControllerRoom;
    
public:
    
    // Clear all the fields in the view.
    void clear();
    
    // Constructor.
    // Parameter : - edit : true if the user is modifiying a room, false is the
    //                       user is creating a new room.
    ViewRoom(const bool edit = false); 
    // Desctructor.
    ~ViewRoom();
    
    // Setters
    void setTitle(const QString& title);
    void setAction(const QString& action);
    void setRemove(const QString& remove);
    void setRoomName(const QString& name);
    void setNbMessage(const quint32 limit);
    void setRoomLogo(const QString& picture);
    void setPrivate(const bool b);
    void setRoomVisibility(const bool b);
    void setInvitation(const bool b);
    
    // Getters
    QString roomName();
    QSet<quint32> roomUsers();
    QSet<quint32> roomAdmins();
    QString userName();
    QString roomLogo();
    quint32 messageLimit();
    bool isRoomPrivate();
    bool isRoomVisible();
    bool isEditing();
    
    // Add the specified user to the view. If isAdmin is true, the user will also
    // be added to the room admins.
    void addUser(quint32 idUser, const QString& userName, const bool isAdmin = false);
    // Remove the selected member.
    void removeUser();
    // Remove the specified user from the room.
    void removeUser(const quint32 userId);
    
public slots:
    // If the radio buttons for the visibility of private rooms are enabled will
    // disable them. Otherwise will enable them.
    void toggleVisibility();
    // Will call removeUser(userId) with the id of the currently selected user
    // in the member list if creating. Will add to the banned user if editing.
    void remove();
    // Will emit create() or edit() signal.
    void action();
    // Will open a window for the user to choose an image from its file system.
    void browseImage();
    // Toggle the adminstration rights of the selected user in the member list.
    void toggleAdmin();

signals:
    // Signal emited when the user wishes to add a new member to the room.
    void add();
    // Signal emited when the user wishes to create a new room.
    void create();
    // Signal emited when the user wishes to apply the changes to a room.
    void edit();
    // Signal emited when the user wishes to cancel the operation.
    void cancel();
    
private:

    // Toggle the administration rights of the user and modify the display of
    // the user in the view (bold if admin, normal otherwise).
    void toggleAdmin(quint32 idUser, const QString& userName);
    
    // Returns the id of the currently selected user in the list.
    quint32 currentSelectedUserId();
    
    // Core elements
    bool editing;
    QList<QLayout*>* layouts;
    QMap<quint32, QString>* _users;
    QMap<quint32, QString>* _admins;
    
    // GUI elements
    QLabel* lbl_title;
    QLabel* lbl_name;
    QLabel* lbl_number;
    QLabel* lbl_logo;
    QLabel* lbl_member;
    
    QLineEdit* ldt_name;
    QLineEdit* ldt_logo;
    QLineEdit* ldt_member;
    
    QSpinBox* sbx_number;
    
    QCheckBox* chk_private;
    QRadioButton* rbt_visible;
    QRadioButton* rbt_onInvitation;
    
    QButtonGroup* bgp_visibility;
    
    QPushButton* btn_browse;
    QPushButton* btn_add;
    QPushButton* btn_remove;
    QPushButton* btn_admin;
    QPushButton* btn_cancel;
    QPushButton* btn_create;
    
    QListView* lst_members;
    QStandardItemModel* sim_members;
};

#endif //CHAT_VIEW_ROOM_H
