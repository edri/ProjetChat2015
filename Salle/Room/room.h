#ifndef CHAT_ROOM_H
#define CHAT_ROOM_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QListView>
#include <QStandardItemModel>
#include <QButtonGroup>

class Room : public QWidget
{
    Q_OBJECT
    
public:

    Room(); 
    ~Room();
    
public slots:
    void toggleVisibility();
    void loadMembers();
    void addMember();

private:

    // GUI elements
    QLabel* lbl_title;
    QLabel* lbl_name;
    QLabel* lbl_number;
    QLabel* lbl_logo;
    QLabel* lbl_membre;
    
    QLineEdit* ldt_name;
    QLineEdit* ldt_logo;
    QLineEdit* ldt_membre;
    
    QSpinBox* sbx_number;
    
    QCheckBox* chk_private;
    QRadioButton* rbt_visible;
    QRadioButton* rbt_onInvitation;
    
    QButtonGroup* bgp_visibility;
    
    QPushButton* btn_browse;
    QPushButton* btn_add;
    QPushButton* btn_cancel;
    QPushButton* btn_create;
    
    QListView* lst_members;
    
    QStandardItemModel* sim_members;
};

#endif //CHAT_ROOM_H
