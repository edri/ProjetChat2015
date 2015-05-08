#include "viewChat.h"
#include "ui_viewChat.h"

ViewChat::ViewChat(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::viewChat)
{
    _ui->setupUi(this);

    _ui->tre_rooms->setIconSize(QSize(30, 30));
    _ui->tre_rooms->setColumnWidth(0, _ui->tre_rooms->width() - 50);
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
    bool alreadyExisting = false;
    quint32 nbRooms = _ui->tre_rooms->topLevelItemCount();

    for (quint32 i = 0; i < nbRooms; ++i)
    {
        if (_ui->tre_rooms->topLevelItem(i)->data(0, Qt::UserRole).toInt() == roomId)
        {
            alreadyExisting = true;
            break;
        }
    }

    if (!alreadyExisting)
    {
        QTreeWidgetItem* roomItem = new QTreeWidgetItem(_ui->tre_rooms);

        roomItem->setData(0, Qt::UserRole, roomId);
        roomItem->setText(0, roomName);
        roomItem->setIcon(0, QIcon(QPixmap::fromImage(roomPicture)));

        roomItem->setData(1, Qt::UserRole, 0);
        roomItem->setFont(1, QFont("MS Shell Dlg 2", 9, QFont::Bold));
        roomItem->setForeground(1, QBrush(QColor(255, 85, 0)));
        roomItem->setTextAlignment(1, Qt::AlignHCenter);
    }
}

void ViewChat::addUserToRoom(const quint32 roomId, const quint32 userId, const QString &userName, const QImage& image, const bool isConnected)
{
    quint32 nbRooms = _ui->tre_rooms->topLevelItemCount();
    bool userAlreadyExisting = false;
    quint32 nbRoomUsers;

    for (int i = 0; i < nbRooms; ++i)
    {
        if (_ui->tre_rooms->topLevelItem(i)->data(0, Qt::UserRole).toInt() == roomId)
        {
            nbRoomUsers = _ui->tre_rooms->topLevelItem(i)->childCount();

            for (quint32 j = 0; j < nbRoomUsers; ++j)
            {
                if (_ui->tre_rooms->topLevelItem(i)->child(j)->data(0, Qt::UserRole).toInt() == userId)
                {
                    userAlreadyExisting = true;
                    break;
                }
            }

            if (!userAlreadyExisting)
            {
                QTreeWidgetItem* userItem = new QTreeWidgetItem(_ui->tre_rooms->topLevelItem(i));
                userItem->setData(0, Qt::UserRole, userId);
                userItem->setText(0, userName);
                userItem->setIcon(0, QIcon(QPixmap::fromImage(image)));
                userItem->setFlags(Qt::NoItemFlags);

                if (isConnected)
                    userItem->setFont(0, QFont("MS Shell Dlg 2", 8, QFont::Bold));

                break;
            }
        }
    }
}

void ViewChat::selectFirstRoom() const
{
    if (_ui->tre_rooms->topLevelItemCount())
        _ui->tre_rooms->setCurrentItem(_ui->tre_rooms->topLevelItem(0));
}

void ViewChat::loadRoomMessage(const quint32 roomId, const quint32 messageId, const QString& userName,
                               const QString& content, const QDateTime& date, const bool isCurrentUsersMessage)
{
    quint32 i, j;
    quint32 nbTopMessageItems = _ui->tre_messages->topLevelItemCount();
    quint32 nbTopRoomItems = _ui->tre_rooms->topLevelItemCount();
    bool createNewDate = false;
    QDate tempDate;

    for (quint32 k = 0; k < nbTopRoomItems; ++k)
    {
        if (_ui->tre_rooms->topLevelItem(k)->data(0, Qt::UserRole).toInt() == roomId)
        {
            if (_ui->tre_rooms->topLevelItem(k)->isSelected())
            {
                QTreeWidgetItem* messageItem = new QTreeWidgetItem();
                messageItem->setData(0, Qt::UserRole, date);
                messageItem->setData(1, Qt::UserRole, messageId);
                messageItem->setText(0, "[" + date.toString("HH:mm") + "] <" + userName + ">");
                messageItem->setText(1, content);

                if (isCurrentUsersMessage)
                {
                    messageItem->setBackgroundColor(0, QColor(234, 239, 245));
                    messageItem->setBackgroundColor(1, QColor(234, 239, 245));
                    messageItem->setFlags(messageItem->flags() | Qt::ItemIsEditable);
                }

                // Select the top-level item in which we will put the new message.
                for (i = 0; i < nbTopMessageItems; ++i)
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

                if (i < nbTopMessageItems && !createNewDate)
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
                _ui->tre_messages->resizeColumnToContents(1);
                _ui->tre_messages->scrollToBottom();
            }
            else
            {
                QTreeWidgetItem* item = _ui->tre_rooms->topLevelItem(k);
                quint32 nbNewMessages = item->data(1, Qt::UserRole).toInt() + 1;

                item->setData(1, Qt::UserRole, nbNewMessages);

                _ui->tre_rooms->topLevelItem(k)->setText(1, "(" + QString::number(nbNewMessages) + ")");
                _ui->tre_rooms->topLevelItem(k)->setForeground(0, QBrush(QColor(255, 85, 0)));
                _ui->tre_rooms->resizeColumnToContents(1);
            }

            break;
        }
    }
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

}

void ViewChat::on_btn_joinRoom_clicked()
{

}

void ViewChat::on_btn_newRoom_clicked()
{
    emit requestOpenRoomModule();
}

void ViewChat::on_tre_rooms_itemSelectionChanged()
{
    for (int i = 0; i < _ui->tre_rooms->topLevelItemCount(); ++i)
    {
        if (_ui->tre_rooms->topLevelItem(i)->isSelected())
        {
            _selectedRoomId = _ui->tre_rooms->topLevelItem(i)->data(0, Qt::UserRole).toInt();

            _ui->tre_rooms->topLevelItem(i)->setFont(0, QFont("MS Shell Dlg 2", 9, QFont::Bold));
            _ui->tre_rooms->topLevelItem(i)->setForeground(0, QBrush(Qt::black));
            _ui->tre_rooms->topLevelItem(i)->setText(1, "");
            _ui->tre_rooms->topLevelItem(i)->setData(1, Qt::UserRole, 0);
            _ui->tre_rooms->expandItem(_ui->tre_rooms->topLevelItem(i));
            _ui->tre_rooms->resizeColumnToContents(1);

            _ui->tre_messages->clear();

            // Request the controller to load the selected room's messages.
            emit requestLoadRoomMessages(_selectedRoomId);
        }
        else
            _ui->tre_rooms->topLevelItem(i)->setFont(0, QFont("MS Shell Dlg 2", 9, QFont::Normal));
    }
}

void ViewChat::on_tre_messages_itemChanged(QTreeWidgetItem* item, int column)
{
    /*item->setBackgroundColor(0, QColor(255, 0, 0));
    item->setBackgroundColor(1, QColor(255, 0, 0));*/

    emit requestEditMessage(item);
}

void ViewChat::on_tre_messages_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    _ui->tre_messages->editItem(item, 1);
}

