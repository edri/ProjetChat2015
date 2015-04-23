#include <QtWidgets>

#include "viewRoom.h"
#include "controllerRoom.h"

#include <iostream>

 ViewRoom:: ViewRoom()
{
    // Setting attributes
    setAttribute(Qt::WA_DeleteOnClose, true);
    
    // Initializazion of core elements
    editing = false;
    layouts = new QList<QLayout*>();
    
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
    
    lsw_members = new QListWidget();
    
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
    vbl_main->addWidget(lsw_members);
    vbl_main->addLayout(hbl_remove);
    vbl_main->addLayout(hbl_visibilityOptions);
    vbl_main->addLayout(hbl_down);
    
    setLayout(vbl_main);
    // Connect signals with public slots.
    /*connect(chk_private, SIGNAL(stateChanged(int )), this, SLOT(toggleVisibility()));
    connect(btn_add, SIGNAL(clicked()), this, SIGNAL(add()));
    connect(ldt_member, SIGNAL(returnPressed()), this, SIGNAL(add()));
    connect(btn_cancel, SIGNAL(clicked()), this, SIGNAL(cancel()));
    connect(btn_remove, SIGNAL(clicked()), this, SIGNAL(remove()));
    connect(btn_create, SIGNAL(clicked()), this, SLOT(action()));*/
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
    delete lsw_members;
    
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
    lsw_members->clear();
    chk_private->setChecked(false);
    rbt_onInvitation->setChecked(false);
    rbt_visible->setChecked(false);
    sbx_number->setValue(50);
}

void ViewRoom::toggleVisibility()
{
    rbt_visible->setEnabled(chk_private->isChecked());
    rbt_onInvitation->setEnabled(chk_private->isChecked());
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

void ViewRoom::loadMembers(const ModelRoom* room)
{    
    for(ModelUser* user : room->getUsers())
    {
        addMember(user->getUserName());
    }
}

void ViewRoom::addMember(const QString name)
{
    lsw_members->addItem(name);
}

void ViewRoom::addMember()
{
    QString memberName = ldt_member->text().trimmed();
    // Check that the name isn't an empty string and that it isn't already in the members list.
    if (!memberName.isEmpty())
    {
        if(lsw_members->findItems(memberName, Qt::MatchExactly).isEmpty())
        {
            // TO DO : Check that the user exists.
            addMember(memberName);
        }
        
        else
        {
            QMessageBox::information(this, tr("Opération impossible") ,tr("Cet utilisateur est déjà membre de cette salle"));
        }
    }
    ldt_member->clear();
}

void ViewRoom::removeMember()
{
    // The room already exists so we check the user really wants to remove members
    // from it. We also must send a command to the server.
    if (editing)
    {
        // Stand in code.
        lsw_members->removeItemWidget(lsw_members->selectedItems().at(0));
    }
    
    // The room isn't created yet, so we don't care if the members are removed and
    // it just need to be removed locally.
    else
    {
        lsw_members->removeItemWidget(lsw_members->selectedItems().at(0));
    }
}

/*void ViewRoom::loadRoom(const ModelRoom* room)
{
    ldt_name->setText(room->getName());
    sbx_number->setValue(room->getLimit());
    ldt_logo->setText(room->getPicture());
    chk_private->setChecked(room->isPrivate());
    if(room->isPrivate())
    {
        rbt_visible->setChecked(room->isVisible());
        rbt_onInvitation->setChecked(!room->isVisible());
    }
}*/

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

void ViewRoom::setVisible(const bool b)
{
    rbt_visible->setChecked(b);
}

void ViewRoom::setInvitation(const bool b)
{
    rbt_onInvitation->setChecked(b);
}
