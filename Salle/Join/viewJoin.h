/*
     * Created by Jan Purro
     *
     * Interface used in order to join a room
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

    ViewJoin(); 
    ~ViewJoin();
    
    void setPublicRooms(const QList<QPair<quint32, QString>>& publicRooms);
    void setPrivateRooms(const QList<QPair<quint32, QString>>& privateRooms);
    
public slots:
    void filterRooms(const QString& substring);
    void checkRoom();

signals:
    // Signal emited when the user wishes to cancel the operation.
    void cancel();
    // Signal emited when the user wishes to join a room.
    void join(quint32 roomId);

private:
    void loadRooms();
    
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
};

#endif //CHAT_JOIN_H
