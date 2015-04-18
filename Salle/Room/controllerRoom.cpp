#include "controllerRoom.h"

ControllerRoom::ControllerRoom()
{
    viewRoom = new ViewRoom();
    
    // Connect signals with public slots.
    connect(viewRoom->btn_add, SIGNAL(clicked()), this, SLOT(addMember()));
    connect(viewRoom->ldt_member, SIGNAL(returnPressed()), this, SLOT(addMember()));
    connect(viewRoom->btn_remove, SIGNAL(clicked()), this, SLOT(removeMember()));
    connect(viewRoom->btn_clear, SIGNAL(clicked()), this, SLOT(removeMembers()));
}

ControllerRoom::~ControllerRoom()
{
    delete viewRoom;
}

void ControllerRoom::showRoom()
{
    // Set the label and button to have the texts corresponding to creation
    viewRoom->lbl_title->setText(tr("Nouvelle Salle"));
    viewRoom->btn_create->setText(tr("Créer"));
    
    viewRoom->clear();
    
    viewRoom->editing = false;
    
    viewRoom->show();
}

void ControllerRoom::showRoom(const quint32 idRoom)
{
    // Set the label and button to have the texts corresponding to edition
    viewRoom->lbl_title->setText(tr("Edition Salle"));
    viewRoom->btn_create->setText(tr("Editer"));
    
    viewRoom->clear();
    
    // Retrieve the room from the id
    ModelRoom* room = model->getRoom(idRoom);
    
    // Initialize the fields with the values of the room.
    loadMembers(room);
    viewRoom->ldt_name->setText(room->getName());
    viewRoom->sbx_number->setValue(room->getLimit());
    viewRoom->ldt_logo->setText(room->getPicture());
    viewRoom->chk_private->setChecked(room->isPrivate());
    if(room->isPrivate())
    {
        viewRoom->rbt_visible->setChecked(room->isVisible());
        viewRoom->rbt_onInvitation->setChecked(!room->isVisible());
    }
    
    viewRoom->editing = true;
    
    viewRoom->show();
}

void ControllerRoom::loadMembers(const ModelRoom* room)
{    
    for(ModelUser* user : room->getUsers())
    {
        addMember(user->getUserName());
    }
}

void ControllerRoom::addMember(const QString name)
{
    QStandardItem* item = new QStandardItem(name);
    item->setEditable(false);
    viewRoom->sim_members->appendRow(item);
}

void ControllerRoom::addMember()
{
    QString memberName = viewRoom->ldt_member->text().trimmed();
    // Check that the name isn't an empty string and that it isn't already in the members list.
    if (!memberName.isEmpty())
    {
        if(viewRoom->sim_members->findItems(memberName).isEmpty())
        {
            addMember(memberName);
        }
        
        else
        {
            QMessageBox::information(viewRoom, tr("Opération impossible") ,tr("Cet utilisateur est déjà membre de cette salle"));
        }
    }
    viewRoom->ldt_member->clear();
}

void ControllerRoom::removeMember()
{
    if (viewRoom->editing)
    {
        
    }
    
    else
    {
        viewRoom->sim_members->removeRow(viewRoom->lst_members->currentIndex().row());
    }
}

void ControllerRoom::removeMembers()
{
    if (viewRoom->editing)
    {
        
    }
    
    else
    {
        viewRoom->sim_members->clear();
    }
    
}
