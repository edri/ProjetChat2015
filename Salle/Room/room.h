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
    QLabel* label_title;
    QLabel* label_name;
    QLabel* label_number;
    QLabel* label_logo;
    QLabel* label_membre;
    
    QLineEdit* ledit_name;
    QLineEdit* ledit_logo;
    QLineEdit* ledit_membre;
    
    QSpinBox* spinb_number;
    
    QCheckBox* chkb_private;
    QRadioButton* radbtn_visible;
    QRadioButton* radbtn_onInvitation;
    
    QButtonGroup* btngrp_visibility;
    
    QPushButton* btn_browse;
    QPushButton* btn_add;
    QPushButton* btn_cancel;
    QPushButton* btn_create;
    
    QListView* list_members;
    
    QStandardItemModel* model_members;
};

#endif //CHAT_ROOM_H
