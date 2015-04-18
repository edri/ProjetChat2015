// View for the creation of new rooms or edition of already existing ones.

#ifndef CHAT_VIEW_ROOM_H
#define CHAT_VIEW_ROOM_H

#include <QtWidgets>

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
    
public slots:
    void toggleVisibility();
    
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
    QPushButton* btn_clear;
    QPushButton* btn_cancel;
    QPushButton* btn_create;
    
    QListView* lst_members;
    QStandardItemModel* sim_members;
};

#endif //CHAT_VIEW_ROOM_H
