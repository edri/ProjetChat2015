// View for the creation of new rooms or edition of already existing ones.

#ifndef CHAT_VIEW_ROOM_H
#define CHAT_VIEW_ROOM_H

#include <QtWidgets>
#include "../../ModeleChator/modelChator.h"

class ControllerRoom;

class ViewRoom : public QWidget
{
    Q_OBJECT
    
    friend ControllerRoom;
    
public:
    
    // Clear all the fields in the widget.
    void clear();

    ViewRoom(); 
    ~ViewRoom();
    
    // Setters
    void setTitle(const QString& title);
    void setAction(const QString& action);
    void setRemove(const QString& remove);
    void setRoomName(const QString& name);
    void setNbMessage(const quint32 limit);
    void setRoomLogo(const QString& picture);
    void setPrivate(const bool b);
    void setVisible(const bool b);
    void setInvitation(const bool b);
    
    //void loadRoom(const ModelRoom* room);
    // Load members of a room into the model of the list view.
    void loadMembers(const ModelRoom* room);
    // Add a member into the model of the list view.
    void addMember(const QString name);
    // Add the current name entered into the members list. 
    void addMember();
    // Remove the selected member.
    void removeMember();
    
public slots:
    void toggleVisibility();
    void action();

signals:
    // Add a new member.
    void add();
    // Remove/kick a memeber.
    void remove();
    // Toggle admin rights for the selected member
    void admin();
    // Create a new room.
    void create(); 
    // Edit the current room
    void edit();
    // Cancel operation
    void cancel();
    
    
private:

    // Core elements
    bool editing;
    QList<QLayout*>* layouts;
    
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
    
    QListWidget* lsw_members;
};

#endif //CHAT_VIEW_ROOM_H
