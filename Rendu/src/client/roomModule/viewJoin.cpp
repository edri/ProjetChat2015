/*
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
    setWindowTitle(tr("Adhésion Salle"));
    setWindowIcon(QIcon("../common/media/img/chator.png"));
    
    // Initializazion of core elements
    layouts = new QList<QLayout*>();
    
    // Initialization of the GUI elements.
    lbl_title = new QLabel(tr("Rejoindre une salle"));
    lbl_title->setFont(QFont(this->font().family(), 
                         this->font().pointSize() * 2, QFont::DemiBold));
    lbl_title->setAlignment(Qt::AlignCenter);
    
    lbl_name = new QLabel(tr("Nom de la salle"));
    lbl_rooms = new QLabel(tr("Résultat(s)"));
    
    ldt_name = new QLineEdit("");
    
    btn_join = new QPushButton(tr("Rejoindre"));
    btn_cancel = new QPushButton(tr("Annuler"));
    
    lst_rooms = new QListView();
    
    sim_rooms = new QStandardItemModel();
    lst_rooms->setModel(sim_rooms);
    
    // Main Layout
    QVBoxLayout* vbl_main = new QVBoxLayout();
    layouts->append(vbl_main);
    vbl_main->addWidget(lbl_title);
    vbl_main->addSpacing(3);
    vbl_main->addWidget(lbl_name);
    vbl_main->addWidget(ldt_name);
    vbl_main->addWidget(lbl_rooms);
    vbl_main->addWidget(lst_rooms);
    vbl_main->addWidget(btn_join);
    vbl_main->addWidget(btn_cancel);
    
    setLayout(vbl_main);
    
    // Connect signals with public slots or signals
    connect(btn_cancel, SIGNAL(clicked()), this, SIGNAL(cancel()));
    connect(btn_join, SIGNAL(clicked()), this, SLOT(checkRoom()));
    connect(ldt_name, SIGNAL(textEdited(const QString&)), this, SLOT(filterRooms(const QString&)));
}

ViewJoin::~ViewJoin()
{
    delete lbl_title;
    delete lbl_name;
    delete lbl_rooms;
    delete ldt_name;
    delete btn_join;
    delete btn_cancel;
    delete lst_rooms;
    delete sim_rooms;
    
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
    // The items won't be editable, will display the user's name and will contain
    // its id in the data field.
    QStandardItem* item;
    for (QPair<quint32, QString> pair : _publicRooms)
    {
        item = new QStandardItem(pair.second);
        item->setEditable(false);
        item->setData(pair.first);
        sim_rooms->appendRow(item);
    }
    
    // The items won't be editable, will display the room's name and will contain
    // its id in the data field. The private rooms are displayed in Bold.
    for (QPair<quint32, QString> pair : _privateRooms)
    {
        item = new QStandardItem(pair.second);
        item->setEditable(false);
        item->setData(pair.first);
        item->setFont(QFont(this->font().family(), 
                      this->font().pointSize(), QFont::Bold));
        sim_rooms->appendRow(item);
    }
}

void ViewJoin::filterRooms(const QString& substring)
{
    // Clears the view
    sim_rooms->clear();
    
    // Similar to loadRooms, but only the rooms containing the given substring
    // will be displayed.
    QStandardItem* item;
    for (QPair<quint32, QString> pair : _publicRooms)
    {
        if(pair.second.contains(substring, Qt::CaseInsensitive))
        {
            item = new QStandardItem(pair.second);
            item->setEditable(false);
            item->setData(pair.first);
            sim_rooms->appendRow(item);
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
                                     sim_rooms->appendRow(item);
        }
    }
}

void ViewJoin::checkRoom()
{
    // If no room was selected by the user, returns.
    if(lst_rooms->selectionModel()->selectedIndexes().isEmpty())
    {
        return;
    }
    
    QStandardItem* item = sim_rooms->item(lst_rooms->currentIndex().row());
    
    emit(join(item->data().toUInt(), item->font().weight() == QFont::Bold));
}

void ViewJoin::closeEvent(QCloseEvent *)
{
    emit reactivateChatWindows();
}
