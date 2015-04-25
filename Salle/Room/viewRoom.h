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
    void setRoomVisibility(const bool b);
    void setInvitation(const bool b);
    void setEditing(bool b);
    
    // Getters
    QString roomName();
    QMap<quint32, QString> roomUsers();
    QMap<quint32, QString> roomAdmins();
    QString userName();
    QString roomLogo();
    bool isEditing();
    
    void toggleAdmin();
    
    void addUser(quint32 idUser, const QString& userName, const bool isAdmin = false);
    // Remove the selected member.
    void removeUser();
    void removeUser(const quint32 userId);
    
public slots:
    void toggleVisibility();
    void willRemove();
    void action();
    void browseImage();

signals:
    // Add a new member.
    void add();
    // Remove/kick a memeber.
    void remove(const quint32 userId);
    // Create a new room.
    void create();
    // Toggle admin rights
    void admin();
    // Edit the current room
    void edit();
    // Cancel operation
    void cancel();
    
private:

    void toggleAdmin(quint32 idUser, const QString& userName);
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
