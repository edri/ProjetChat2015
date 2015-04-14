#ifndef CHAT_JOIN_H
#define CHAT_JOIN_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QListView>
#include <QStandardItemModel>

class Join : public QWidget
{
    Q_OBJECT
    
public:

    Join(); 
    ~Join();
    
public slots:

private:

    // GUI elements
    QLabel* label_title;
    QLabel* label_name;
    QLabel* label_rooms;
    
    QLineEdit* ledit_name;
    
    QPushButton* btn_join;
    QPushButton* btn_cancel;
    
    QListView* list_rooms;
    
    QStandardItemModel* model_rooms;
};

#endif //CHAT_JOIN_H
