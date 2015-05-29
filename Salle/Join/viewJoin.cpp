/*
     * Created by Miguel Santamaria
     *
     * Implements viewJoin.h and configure element
     * in the GUI.
*/

#include <QVBoxLayout>
#include <QStandardItem>
#include <QFont>
#include <QDebug>
#include "viewJoin.h"

ViewJoin::ViewJoin()
{    
    // Setting attributes
    setAttribute(Qt::WA_DeleteOnClose, true);
    
    // Initializazion of core elements
    layouts = new QList<QLayout*>();
    
    // Initialization of the GUI elements.
    label_title = new QLabel(tr("Rejoindre une salle"));
    label_title->setFont(QFont(this->font().family(), 
                         this->font().pointSize() * 2, QFont::DemiBold));
    label_title->setAlignment(Qt::AlignCenter);
    
    label_name = new QLabel(tr("Nom de la salle"));
    label_rooms = new QLabel(tr("Résultat(s)"));
    
    ledit_name = new QLineEdit("");
    
    btn_join = new QPushButton(tr("Rejoindre"));
    btn_cancel = new QPushButton(tr("Annuler"));
    
    list_rooms = new QListView();
    
    model_rooms = new QStandardItemModel();
    list_rooms->setModel(model_rooms);
    
    // Main Layout
    QVBoxLayout* vbl_main = new QVBoxLayout();
    layouts->append(vbl_main);
    vbl_main->addWidget(label_title);
    vbl_main->addSpacing(3);
    vbl_main->addWidget(label_name);
    vbl_main->addWidget(ledit_name);
    vbl_main->addWidget(label_rooms);
    vbl_main->addWidget(list_rooms);
    vbl_main->addWidget(btn_join);
    vbl_main->addWidget(btn_cancel);
    
    setLayout(vbl_main);
    
    
    connect(btn_cancel, SIGNAL(clicked()), this, SIGNAL(cancel()));
    connect(btn_join, SIGNAL(clicked()), this, SLOT(checkRoom()));
    connect(ledit_name, SIGNAL(textEdited(const QString&)), this, SLOT(filterRooms(const QString&)));
}

ViewJoin::~ViewJoin()
{
    delete label_title;
    delete label_name;
    delete label_rooms;
    delete ledit_name;
    delete btn_join;
    delete btn_cancel;
    delete list_rooms;
    delete model_rooms;
    
    // Delete layouts.
    for (QLayout* l : *layouts)
    {
        delete l;
    }
    delete layouts;
}

void ViewJoin::setPublicRooms(const QList<QPair<quint32, QString>>& publicRooms)
{
    _publicRooms = publicRooms;
}

void ViewJoin::setPrivateRooms(const QList<QPair<quint32, QString>>& privateRooms)
{
    _privateRooms = privateRooms;
}

void ViewJoin::loadRooms()
{
    QStandardItem* item;
    for (QPair<quint32, QString> pair : _publicRooms)
    {
        item = new QStandardItem(pair.second);
        item->setEditable(false);
        item->setData(pair.first);
        model_rooms->appendRow(item);
    }
    
    for (QPair<quint32, QString> pair : _privateRooms)
    {
        item = new QStandardItem(pair.second);
        item->setEditable(false);
        item->setData(pair.first);
        item->setFont(QFont(this->font().family(), 
                      this->font().pointSize(), QFont::Bold));
        model_rooms->appendRow(item);
    }
}

void ViewJoin::filterRooms(const QString& substring)
{
    model_rooms->clear();
    QStandardItem* item;
    for (QPair<quint32, QString> pair : _publicRooms)
    {
        if(pair.second.contains(substring, Qt::CaseInsensitive))
        {
            item = new QStandardItem(pair.second);
            item->setEditable(false);
            item->setData(pair.first);
            model_rooms->appendRow(item);
        }
    }
    
    for (QPair<quint32, QString> pair : _privateRooms)
    {
        if(pair.second.contains(substring, Qt::CaseInsensitive))
        {
            item = new QStandardItem(pair.second);
            item->setEditable(false);
            item->setData(pair.first);
            item->setFont(QFont(this->font().family(), 
                          this->font().pointSize(), QFont::Bold));
                                     model_rooms->appendRow(item);
        }
    }
}

void ViewJoin::checkRoom()
{
    // If no room was selected by the user, returns.
    if(list_rooms->selectionModel()->selectedIndexes().isEmpty())
    {
        return;
    }
    
    QStandardItem* item = model_rooms->item(list_rooms->currentIndex().row());
    
    emit(join(item->data().toUInt()));
}
