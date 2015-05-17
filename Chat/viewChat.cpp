#include "viewChat.h"
#include "ui_viewChat.h"

ViewChat::ViewChat(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::viewChat),
    _isEditingMessage(false),
    _menu(new QMenu(this))
{
    _ui->setupUi(this);

    _ui->btn_edit->hide();
    _ui->btn_delete->hide();

    _ui->tre_rooms->setIconSize(QSize(30, 30));
    _ui->tre_rooms->setColumnWidth(0, _ui->tre_rooms->width() - 50);

    _ui->tre_messages->expandAll();
    _ui->tre_messages->resizeColumnToContents(3);
    _ui->tre_messages->header()->close();

    _ui->ldt_message->setFocus();

    _ui->tre_messages->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_ui->tre_messages, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMessage(const QPoint&)));
}

ViewChat::~ViewChat()
{
    delete _ui;
    delete _menu;
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

    for (quint32 i = 0; i < nbRooms; ++i)
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
                               const QString& content, const QDateTime& date, const QDateTime& lastUpdateDate,
                               const bool isCurrentUsersMessage, const bool edited)
{
    quint32 i, j;
    quint32 nbTopMessageItems = _ui->tre_messages->topLevelItemCount();
    quint32 nbTopRoomItems = _ui->tre_rooms->topLevelItemCount();
    quint32 nbChildren;
    /*bool createNewDate = false;
    QDate tempDate;*/

    for (quint32 k = 0; k < nbTopRoomItems; ++k)
    {
        if (_ui->tre_rooms->topLevelItem(k)->data(0, Qt::UserRole).toInt() == roomId)
        {
            if (_ui->tre_rooms->topLevelItem(k)->isSelected())
            {
                if (edited)
                {
                    // Select the message to update.
                    for (i = 0; i < nbTopMessageItems; ++i)
                    {
                        nbChildren = _ui->tre_messages->topLevelItem(i)->childCount();

                        for (j = 0; j < nbChildren; ++j)
                        {
                            if (_ui->tre_messages->topLevelItem(i)->child(j)->data(1, Qt::UserRole) == messageId)
                            {
                                _isEditingMessage = true;
                                _ui->tre_messages->topLevelItem(i)->child(j)->setText(1, content);
                                _ui->tre_messages->topLevelItem(i)->child(j)->setText(2, "[Edité le " + lastUpdateDate.toString("dd.MM.yyyy à HH:mm") + "]");
                                _ui->tre_messages->topLevelItem(i)->child(j)->setTextColor(2, QColor(192, 192, 192));
                                _ui->tre_messages->topLevelItem(i)->child(j)->setFont(2, QFont("MS Shell Dlg 2", 9, -1, true));
                            }
                        }
                    }
                }
                else
                {
                    QTreeWidgetItem* messageItem = new QTreeWidgetItem();
                    messageItem->setData(0, Qt::UserRole, date);
                    messageItem->setData(1, Qt::UserRole, messageId);
                    messageItem->setData(2, Qt::UserRole, isCurrentUsersMessage);
                    messageItem->setText(0, "[" + date.toString("HH:mm") + "] <" + userName + ">");
                    messageItem->setText(1, content);

                    if (lastUpdateDate != date)
                    {
                        messageItem->setText(2, "[" + tr("Edité le ") + lastUpdateDate.toString("dd.MM.yyyy à HH:mm") + "]");
                        messageItem->setTextColor(2, QColor(192, 192, 192));
                        messageItem->setFont(2, QFont("MS Shell Dlg 2", 9, -1, true));
                    }

                    if (isCurrentUsersMessage)
                    {
                        messageItem->setBackgroundColor(0, QColor(234, 239, 245));
                        messageItem->setBackgroundColor(1, QColor(234, 239, 245));
                        messageItem->setBackgroundColor(2, QColor(234, 239, 245));
                        messageItem->setFlags(messageItem->flags() | Qt::ItemIsEditable);
                    }

                    // Select the top-level item in which we will put the new message.
                    /*for (i = 0; i < nbTopMessageItems; ++i)
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
                    }*/

                    if (nbTopMessageItems && QDate::fromString(_ui->tre_messages->topLevelItem(nbTopMessageItems - 1)->text(0), "dd.MM.yyyy") == date.date())
                    /*if (i < nbTopMessageItems && !createNewDate)*/
                    {
                        // Insert the message in the right place in the top-level item.
                        /*for (j = 0; j < (quint32)_ui->tre_messages->topLevelItem(i)->childCount(); ++j)
                        {
                            if (_ui->tre_messages->topLevelItem(i)->child(j)->data(0, Qt::UserRole).toDateTime() > date)
                                break;
                        }

                        _ui->tre_messages->topLevelItem(i)->insertChild(j, messageItem);*/
                        _ui->tre_messages->topLevelItem(nbTopMessageItems - 1)->insertChild(_ui->tre_messages->topLevelItem(nbTopMessageItems - 1)->childCount(), messageItem);
                    }
                    else
                    {
                        // Create the top-level item, and insert the message in the first subrow.
                        QTreeWidgetItem* dateItem = new QTreeWidgetItem();
                        dateItem->setData(0, Qt::UserRole, date);
                        dateItem->setText(0, date.toString("dd.MM.yyyy"));
                        dateItem->setFont(0, QFont("MS Shell Dlg 2", 9, QFont::Bold));

                        /*_ui->tre_messages->insertTopLevelItem(i, dateItem);
                        _ui->tre_messages->topLevelItem(i)->insertChild(0, messageItem);*/
                        _ui->tre_messages->insertTopLevelItem(nbTopMessageItems, dateItem);
                        _ui->tre_messages->topLevelItem(nbTopMessageItems)->insertChild(0, messageItem);

                        _ui->tre_messages->expandItem(dateItem);
                    }

                    _ui->tre_messages->resizeColumnToContents(0);
                    _ui->tre_messages->scrollToBottom();
                }

                _ui->tre_messages->resizeColumnToContents(1);
                _ui->tre_messages->resizeColumnToContents(2);
            }
            else if (!edited)
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

void ViewChat::userStatusChanged(const quint32 userId, const bool isConnected) const
{
    quint32 nbTopRoomItems = _ui->tre_rooms->topLevelItemCount();
    quint32 nbRoomUsers;

    for (quint32 i = 0; i < nbTopRoomItems; ++i)
    {
        nbRoomUsers = _ui->tre_rooms->topLevelItem(i)->childCount();

        for (quint32 j = 0; j < nbRoomUsers; ++j)
        {
            if (_ui->tre_rooms->topLevelItem(i)->child(j)->data(0, Qt::UserRole).toInt() == userId)
            {
                if (isConnected)
                    _ui->tre_rooms->topLevelItem(i)->child(j)->setFont(0, QFont("MS Shell Dlg 2", 8, QFont::Bold));
                else
                    _ui->tre_rooms->topLevelItem(i)->child(j)->setFont(0, QFont("MS Shell Dlg 2", 8));
            }
        }
    }
}

void ViewChat::updateButtons(const bool isAdmin) const
{
    if (isAdmin)
    {
        _ui->btn_edit->show();
        _ui->btn_delete->show();
    }
    else
    {
        _ui->btn_edit->hide();
        _ui->btn_delete->hide();
    }
}

void ViewChat::deleteMessage(const quint32 messageId) const
{
    quint32 nbMessageDates = _ui->tre_messages->topLevelItemCount();
    quint32 nbMessages;

    for (quint32 i = 0; i < nbMessageDates; ++i)
    {
        nbMessages = _ui->tre_messages->topLevelItem(i)->childCount();

        for (quint32 j = 0; j < nbMessages; ++j)
        {
            int a = _ui->tre_messages->topLevelItem(i)->child(j)->data(1, Qt::UserRole).toInt();
            if (a == messageId)
            {
                _ui->tre_messages->topLevelItem(i)->takeChild(j);

                // If the top-level item has no child anymore, we take it too.
                if (--nbMessages == 0)
                    _ui->tre_messages->takeTopLevelItem(i);

                return;
            }
        }
    }
}

void ViewChat::deleteRoom(const quint32 roomId) const
{
    quint32 nbRooms = _ui->tre_rooms->topLevelItemCount();

    for (quint32 i = 0; i < nbRooms; ++i)
    {
        if (_ui->tre_rooms->topLevelItem(i)->data(0, Qt::UserRole).toInt() == roomId)
        {
            _ui->tre_messages->clear();
            _ui->tre_rooms->takeTopLevelItem(i);
            break;
        }
    }
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
    int ret = QMessageBox::warning(this, tr("Attention"),
                                   tr("Êtes-vous sûr de vouloir quitter cette salle ?"),
                                   tr("Oui"), tr("Non"));

    // "Yes" pressed.
    if (ret == 0)
        emit requestLeaveRoom(_ui->tre_rooms->selectedItems().at(0)->data(0, Qt::UserRole).toInt());
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
    if (column == 1 && !_isEditingMessage)
        emit requestEditMessage(item);

    _isEditingMessage = false;
}

void ViewChat::on_tre_messages_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);

    _ui->tre_messages->editItem(item, 1);
}

void ViewChat::on_actionQuitter_triggered()
{
    exit(0);
}

void ViewChat::on_btn_edit_clicked()
{

}

void ViewChat::on_btn_delete_clicked()
{
    int ret = QMessageBox::warning(this, tr("Attention"),
                                   tr("Êtes-vous sûr de vouloir supprimer cette salle ?"),
                                   tr("Oui"), tr("Non"));

    // "Yes" pressed.
    if (ret == 0)
        emit requestDeleteRoom(_ui->tre_rooms->selectedItems().at(0)->data(0, Qt::UserRole).toInt());
}

void ViewChat::showContextMessage(const QPoint &pos)
{
    // The selected item must be the current user's property.
    if (_ui->tre_messages->selectedItems().at(0)->data(2, Qt::UserRole).toBool())
    {
        QAction* editAct = _menu->addAction(QIcon("img/edit.png"), tr("Editer"));
        QAction* delAct = _menu->addAction(QIcon("img/delete.png"), tr("Supprimer"));

        QAction* act = _menu->exec(_ui->tre_messages->viewport()->mapToGlobal(pos));

        if (act == editAct)
        {
            _ui->tre_messages->editItem(_ui->tre_messages->selectedItems().at(0), 1);
            delete delAct;
        }
        else if (act == delAct)
        {
            emit requestDeleteMessage(_ui->tre_messages->selectedItems().at(0)->data(1, Qt::UserRole).toInt());
            delete editAct;
        }
        else
        {
            delete editAct;
            delete delAct;
        }

        delete act;
    }
}
