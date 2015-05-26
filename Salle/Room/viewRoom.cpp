/*
 * File : viewRoom.cpp
 * Project : ProjetChat2015
 * Author(s) : Jan Purro
 * Last Modified : 25.04.2015 14:25 by Jan Purro
 * Description : Implementation of the room view.
 */


#include <QtWidgets>

#include "viewRoom.h"
#include "controllerRoom.h"

 ViewRoom:: ViewRoom(const bool edit)
{
    // Setting attributes
    
    // The view will be destroyed if it's closed.
    setAttribute(Qt::WA_DeleteOnClose, true);
    
    // Initializazion of core elements
    editing = edit;
    layouts = new QList<QLayout*>();
    _users = new QMap<quint32, QString>;
    _admins = new QMap<quint32, QString>;
    
    // Initialization of the GUI elements.
    lbl_title = new QLabel("");
    lbl_title->setFont(QFont(this->font().family(), 
                         this->font().pointSize() * 2, QFont::DemiBold));
    lbl_title->setAlignment(Qt::AlignCenter);
    
    lbl_name = new QLabel(tr("* Nom de la salle"));
    lbl_number = new QLabel(tr("Nombre maximal de messages stockés"));
    lbl_logo = new QLabel(tr("Logo"));
    lbl_member = new QLabel(tr("Membres"));
    
    ldt_name = new QLineEdit("");
    ldt_logo = new QLineEdit("");
    ldt_member = new QLineEdit("");
    ldt_member->setPlaceholderText(tr("Entrez le nom du nouveau member"));
    
    sbx_number = new QSpinBox();
    sbx_number->setRange(0, 10000);
    sbx_number->setSingleStep(50);
    sbx_number->setValue(50);
    
    chk_private = new QCheckBox(tr("Salle Privée"));
    
    rbt_visible = new QRadioButton(tr("Visible par tous"));
    rbt_onInvitation = new QRadioButton(tr("Rejoignable sur invitation"));
    
    rbt_visible->setEnabled(false);
    rbt_onInvitation->setEnabled(false);
    
    rbt_visible->setVisible(true);
    rbt_onInvitation->setVisible(true);
    
    bgp_visibility = new QButtonGroup();
    bgp_visibility->addButton(rbt_visible);
    bgp_visibility->addButton(rbt_onInvitation);
    bgp_visibility->setExclusive(true);
    
    btn_browse = new QPushButton(tr("Parcourir..."));
    btn_add = new QPushButton(tr("Ajouter"));
    btn_remove = new QPushButton(tr(""));
    btn_admin = new QPushButton(tr("Admin"));
    btn_cancel = new QPushButton(tr("Annuler"));
    btn_create = new QPushButton("");
    
    lst_members = new QListView();
    
    sim_members = new QStandardItemModel();
    lst_members->setModel(sim_members);
    
    // Placing the elements inside layouts.
    
    QHBoxLayout* hbl_browse = new QHBoxLayout();
    layouts->append(hbl_browse);
    hbl_browse->addWidget(ldt_logo);
    hbl_browse->addWidget(btn_browse);
    
    QHBoxLayout* hbl_member = new QHBoxLayout();
    layouts->append(hbl_member);
    hbl_member->addWidget(ldt_member);
    hbl_member->addWidget(btn_add);
    
    QHBoxLayout* hbl_remove = new QHBoxLayout();
    layouts->append(hbl_remove);
    hbl_remove->addWidget(btn_remove);
    hbl_remove->addWidget(btn_admin);
    
    QHBoxLayout* hbl_down = new QHBoxLayout();
    layouts->append(hbl_down);
    hbl_down->addWidget(btn_cancel);
    hbl_down->addWidget(btn_create);
    
    QVBoxLayout* vbl_visibilityButtons = new QVBoxLayout();
    layouts->append(vbl_visibilityButtons);
    vbl_visibilityButtons->addWidget(rbt_visible);
    vbl_visibilityButtons->addWidget(rbt_onInvitation);
    
    QHBoxLayout* hbl_visibilityOptions = new QHBoxLayout();
    layouts->append(hbl_visibilityOptions);
    hbl_visibilityOptions->addWidget(chk_private);
    hbl_visibilityOptions->addLayout(vbl_visibilityButtons);
    
    // Main layout.
    QVBoxLayout* vbl_main = new QVBoxLayout();
    layouts->append(vbl_main);
    vbl_main->addWidget(lbl_title);
    vbl_main->addSpacing(3);
    vbl_main->addWidget(lbl_name);
    vbl_main->addWidget(ldt_name);
    vbl_main->addWidget(lbl_number);
    vbl_main->addWidget(sbx_number);
    vbl_main->addWidget(lbl_logo);
    vbl_main->addLayout(hbl_browse);
    vbl_main->addWidget(lbl_member);
    vbl_main->addLayout(hbl_member);
    vbl_main->addWidget(lst_members);
    vbl_main->addLayout(hbl_remove);
    vbl_main->addLayout(hbl_visibilityOptions);
    vbl_main->addLayout(hbl_down);
    
    setLayout(vbl_main);
    // Connect signals with public slots or signals
    connect(chk_private, SIGNAL(stateChanged(int )), this, SLOT(toggleVisibility()));
    connect(btn_add, SIGNAL(clicked()), this, SIGNAL(add()));
    connect(ldt_member, SIGNAL(returnPressed()), this, SIGNAL(add()));
    connect(btn_cancel, SIGNAL(clicked()), this, SIGNAL(cancel()));
    connect(btn_remove, SIGNAL(clicked()), this, SLOT(remove()));
    connect(btn_create, SIGNAL(clicked()), this, SLOT(action()));
    connect(btn_admin, SIGNAL(clicked()), this, SLOT(toggleAdmin()));
    connect(btn_browse, SIGNAL(clicked()), this, SLOT(browseImage()));
}

ViewRoom::~ ViewRoom()
{
    // Delete GUI elements
    delete lbl_title;
    delete lbl_name;
    delete lbl_number;
    delete lbl_logo;
    delete lbl_member;
    delete ldt_name;
    delete ldt_logo;
    delete ldt_member;
    delete sbx_number;
    delete chk_private;
    delete rbt_visible;
    delete rbt_onInvitation;
    delete bgp_visibility;
    delete btn_browse;
    delete btn_add;
    delete btn_cancel;
    delete btn_create;
    delete lst_members;
    delete sim_members;
    delete _users;
    delete _admins;
    
    
    // Delete layouts.
    for (QLayout* l : *layouts)
    {
        delete l;
    }
    delete layouts;
}

void ViewRoom::clear()
{
    ldt_logo->clear();
    ldt_member->clear();
    ldt_name->clear();
    sim_members->clear();
    chk_private->setChecked(false);
    rbt_onInvitation->setChecked(false);
    rbt_visible->setChecked(false);
    sbx_number->setValue(50);
}

void ViewRoom::toggleVisibility()
{
    rbt_visible->setEnabled(chk_private->isChecked());
    rbt_onInvitation->setEnabled(chk_private->isChecked());
    // If neither visibility option is selected and the checkbox for private room
    // is checked the visible room option is checked by default.
    if (!bgp_visibility->checkedButton() && chk_private->isChecked())
    {
        rbt_visible->setChecked(true);
    }
}

void ViewRoom::setTitle(const QString& title)
{
    lbl_title->setText(title);
}

void ViewRoom::setAction(const QString& action)
{
    btn_create->setText(action);
}


void ViewRoom::setRemove(const QString& remove)
{
    btn_remove->setText(remove);
}

void ViewRoom::action()
{
    if(editing)
    {
        emit(edit());
    }
    
    else
    {
        emit(create());
    }
}

void ViewRoom::removeUser()
{   
    sim_members->removeRow(lst_members->currentIndex().row());
}

void ViewRoom::removeUser(const quint32 userId)
{
    removeUser();
    _users->remove(userId);
    if(_users->find(userId) != _users->end())
    {
        _admins->remove(userId);
    }
}

void ViewRoom::setRoomName(const QString& name)
{
    ldt_name->setText(name);
}

void ViewRoom::setNbMessage(const quint32 limit)
{
    sbx_number->setValue(limit);
}

void ViewRoom::setRoomLogo(const QString& picture)
{
    ldt_logo->setText(picture);
}

void ViewRoom::setPrivate(const bool b)
{
    chk_private->setChecked(b);
}

void ViewRoom::setRoomVisibility(const bool b)
{
    rbt_visible->setChecked(b);
}

void ViewRoom::setInvitation(const bool b)
{
    rbt_onInvitation->setChecked(b);
}

QString ViewRoom::roomName()
{
    return ldt_name->text().trimmed();
}

QString ViewRoom::roomLogo()
{
    // Could be a problem if the image name starts, ends or is only composed of
    // space characters, but that would be really weird.
    return ldt_logo->text().trimmed();
}

quint32 ViewRoom::messageLimit()
{
    return sbx_number->value();
}

bool ViewRoom::isRoomPrivate()
{
    return chk_private->isChecked();
}
bool ViewRoom::isRoomVisible()
{
    return !chk_private->isChecked() || rbt_visible->isChecked();
}

QSet<quint32> ViewRoom::roomUsers()
{
    QSet<quint32> ids;
    for (quint32 id : _users->keys())
    {
        ids.insert(id);
    }
    
    return ids;
}

QSet<quint32> ViewRoom::roomAdmins()
{
    QSet<quint32> ids;
    for (quint32 id : _admins->keys())
    {
        ids.insert(id);
    }
    
    return ids;
}

QString ViewRoom::userName()
{
    return ldt_member->text().trimmed();
}

bool ViewRoom::isEditing()
{
    return editing;
}

void ViewRoom::toggleAdmin()
{
    quint32 userId = currentSelectedUserId();
    toggleAdmin(userId, _users->value(userId));
}

quint32 ViewRoom::currentSelectedUserId()
{
    QModelIndex currentMember = lst_members->selectionModel()->currentIndex();
    QString userName = sim_members->itemFromIndex(currentMember)->text();
    return _users->key(userName);
}

void ViewRoom::toggleAdmin(quint32 idUser, const QString& userName)
{
    // First search the user amongst the admins 
    QMap<quint32, QString>::Iterator user = _admins->find(idUser);
    
    // If the user isn't an admin
    if (user == _admins->end())
    {
        
        // Used to verify that the user really exists but this case shoudln't
        // happen.
        /*user = _users->find(idUser);
        if (user == _users->end())
        {
            // Throw exception ?
            return;
        }*/
        
        // Add to the admins and change the display.
        _admins->insert(idUser, userName);
        
        QList<QStandardItem*> user = sim_members->findItems(userName);
        user.at(0)->setFont(QFont(this->font().family(), 
                            this->font().pointSize(), QFont::Bold));
    
    }
    
    else
    {
        // Remove from the admins and change the display
        _admins->remove(idUser);
        
        QList<QStandardItem*> user = sim_members->findItems(userName);
        user.at(0)->setFont(QFont(this->font().family(), 
                            this->font().pointSize(), QFont::Normal));
    }
}

void ViewRoom::addUser(quint32 idUser, const QString& userName, const bool isAdmin)
{
    // If isAdmin is true :
    // If the user was not already inserted before, the user is inserted and 
    // made into admin.
    // If the user was already inserted but wasn't an admin, the user is
    // mad into an admin.
    // If the user was already inserted and already an admin, nothing is done.
    // If isAdmin is flase :
    // The use is inserted if it was not already.
    
    if (_users->find(idUser) == _users->end())
    {
        // The model destroys all its items when destroyed.
        QStandardItem* item = new QStandardItem(userName);
        item->setEditable(false);
        sim_members->appendRow(item);
        _users->insert(idUser, userName);
    }
    
    if (isAdmin && _admins->find(idUser) == _admins->end())
    {
        toggleAdmin(idUser, userName);
    }
    
    ldt_member->clear();
}

void ViewRoom::browseImage()
{
    ldt_logo->setText(QFileDialog::getOpenFileName(this, tr("Open Image"), tr("~"), tr("Image Files (*.png *.jpg)")));
}

void ViewRoom::remove()
{
    if (!editing)
    {
        removeUser(currentSelectedUserId());
    }
    
    else
    {
        // Add to the banned users.
        // Will not be implemented...
    }
}

void ViewRoom::disablePrivacy()
{
    chk_private->setDisabled(true);
    rbt_visible->setDisabled(true);
    rbt_onInvitation->setDisabled(true);
}
