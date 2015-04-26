#include "viewChat.h"
#include "ui_viewChat.h"

ViewChat::ViewChat(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::viewChat)
{
    _ui->setupUi(this);

    _ui->tre_rooms->setIconSize(QSize(30, 30));
    _ui->tre_messages->expandAll();
    _ui->tre_messages->resizeColumnToContents(0);
    _ui->tre_messages->header()->close();
    _ui->ldt_message->setFocus();
}

ViewChat::~ViewChat()
{
    delete _ui;
}

void ViewChat::setConnectedAsText(const QString& user)
{
    _ui->lbl_connectedAs->setText("Connecté en tant que <b>" + user + "</b>.");
}

void ViewChat::addRoom(const quint32 roomId, const QString& roomName, const QImage& roomPicture)
{
    QTreeWidgetItem* roomItem = new QTreeWidgetItem(_ui->tre_rooms);
    roomItem->setData(0, Qt::UserRole, roomId);
    roomItem->setText(0, roomName);
    roomItem->setIcon(0, QIcon(QPixmap::fromImage(roomPicture)));
}

void ViewChat::addUserToRoom(const quint32 roomId, const quint32 userId, const QString &userName, const QImage& image, const bool isConnected)
{
    for (int i = 0; i < _ui->tre_rooms->topLevelItemCount(); ++i)
    {
        if (_ui->tre_rooms->topLevelItem(i)->data(0, Qt::UserRole).toInt() == roomId)
        {
            QTreeWidgetItem* userItem = new QTreeWidgetItem(_ui->tre_rooms->topLevelItem(i));
            userItem->setData(0, Qt::UserRole, userId);
            userItem->setText(0, userName);
            userItem->setIcon(0, QIcon(QPixmap::fromImage(image)));

            if (isConnected)
                userItem->setFont(0, QFont("MS Shell Dlg 2", 8, QFont::Bold));

            break;
        }
    }
}

void ViewChat::selectFirstRoom() const
{
    if (_ui->tre_rooms->topLevelItemCount())
        _ui->tre_rooms->setCurrentItem(_ui->tre_rooms->topLevelItem(0));
}

void ViewChat::loadRoomMessage(const quint32 messageId, const QString& userName, const QString& content, const QDateTime& date)
{
    quint32 i, j;
    quint32 nbTopItems = _ui->tre_messages->topLevelItemCount();
    bool createNewDate = false;
    QDate tempDate;

    QTreeWidgetItem* messageItem = new QTreeWidgetItem();
    messageItem->setData(0, Qt::UserRole, date);
    messageItem->setData(1, Qt::UserRole, messageId);
    messageItem->setText(0, "[" + date.toString("HH:mm") + "] <" + userName + ">");
    messageItem->setText(1, content);

    // Select the top-level item in which we will put the new message.
    for (i = 0; i < nbTopItems; ++i)
    {
        tempDate = QDate::fromString(_ui->tre_messages->topLevelItem(i)->text(0), "dd.MM.yyyy");

        if (tempDate == date.date())
            break;
        else if (tempDate > date.date())
        {
            // The top-level item doesn't exist yet, we must create it.
            createNewDate = true;
            break;
        }
    }

    if (i < nbTopItems && !createNewDate)
    {
        // Insert the message in the right place in the top-level item.
        for (j = 0; j < (quint32)_ui->tre_messages->topLevelItem(i)->childCount(); ++j)
        {
            if (_ui->tre_messages->topLevelItem(i)->child(j)->data(0, Qt::UserRole).toDateTime() > date)
                break;
        }

        _ui->tre_messages->topLevelItem(i)->insertChild(j, messageItem);
    }
    else
    {
        // Create the top-level item, and insert the message in the first subrow.
        QTreeWidgetItem* dateItem = new QTreeWidgetItem();
        dateItem->setData(0, Qt::UserRole, date);
        dateItem->setText(0, date.toString("dd.MM.yyyy"));

        _ui->tre_messages->insertTopLevelItem(i, dateItem);
        _ui->tre_messages->topLevelItem(i)->insertChild(0, messageItem);
        _ui->tre_messages->expandItem(dateItem);
    }

    _ui->tre_messages->resizeColumnToContents(0);
}

QString ViewChat::getMessageText() const
{
    return _ui->ldt_message->text();
}

quint32 ViewChat::getSelectedRoomId() const
{
    return _selectedRoomId;
}

void ViewChat::on_btn_send_clicked()
{
    if (!_ui->ldt_message->text().trimmed().isEmpty())
    {
        emit requestSendMessage();

        _ui->ldt_message->clear();
    }
}

void ViewChat::on_ldt_message_returnPressed()
{
    _ui->btn_send->click();
}

void ViewChat::on_btn_leaveRoom_clicked()
{
    quint32 d = _ui->tre_rooms->selectedItems().at(0)->data(0, Qt::UserRole).toInt();
    _ui->ldt_message->setText(QString::number(d));
}

void ViewChat::on_btn_joinRoom_clicked()
{

}

void ViewChat::on_btn_newRoom_clicked()
{

}

void ViewChat::on_tre_rooms_itemSelectionChanged()
{
    for (int i = 0; i < _ui->tre_rooms->topLevelItemCount(); ++i)
    {
        if (_ui->tre_rooms->topLevelItem(i)->isSelected())
        {
            _selectedRoomId = _ui->tre_rooms->topLevelItem(i)->data(0, Qt::UserRole).toInt();

            _ui->tre_rooms->topLevelItem(i)->setFont(0, QFont("MS Shell Dlg 2", 9, QFont::Bold));
            _ui->tre_rooms->expandItem(_ui->tre_rooms->topLevelItem(i));

            _ui->tre_messages->clear();

            // Request the controller to load the selected room's messages.
            emit requestLoadRoomMessages(_selectedRoomId);
        }
        else
            _ui->tre_rooms->topLevelItem(i)->setFont(0, QFont("MS Shell Dlg 2", 9, QFont::Normal));
    }
}
