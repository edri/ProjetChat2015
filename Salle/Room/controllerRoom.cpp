#include "controllerRoom.h"

ControllerRoom::ControllerRoom(ModelChator* model, ModelUser* const user)
{
    this->model = model;
    this->user = user;
    
    // Initialize the view at nullptr.
    viewRoom = nullptr;
    viewJoin = nullptr;
}

void ControllerRoom::connectViewRoom()
{
    // Connect signals with public slots.
    connect(viewRoom->btn_add, SIGNAL(clicked()), this, SLOT(addMember()));
    connect(viewRoom->ldt_member, SIGNAL(returnPressed()), this, SLOT(addMember()));
    connect(viewRoom->btn_remove, SIGNAL(clicked()), this, SLOT(removeMember()));
    connect(viewRoom->btn_clear, SIGNAL(clicked()), this, SLOT(removeMembers()));
    connect(viewRoom->btn_cancel, SIGNAL(clicked()), this, SLOT(cancelRoom()));
    connect(viewRoom->btn_create, SIGNAL(clicked()), this, SLOT(actionRoom()));
}

void ControllerRoom::connectViewJoin()
{
    connect(viewJoin->btn_cancel, SIGNAL(clicked()), this, SLOT(cancelJoin()));
}

void ControllerRoom::actionRoom()
{
    if(viewRoom->editing)
    {
        editRoom();
    }
    
    else
    {
        createRoom();
    }
}

void ControllerRoom::showRoom()
{
    viewRoom = new ViewRoom();
    connectViewRoom();
    
    // Set the label and button to have the texts corresponding to creation
    viewRoom->lbl_title->setText(tr("Nouvelle Salle"));
    viewRoom->btn_create->setText(tr("Créer"));
    
    // Add the user as a member of the room.
    addMember(user->getUserName());
    
    viewRoom->show();
}

void ControllerRoom::showRoom(const quint32 idRoom)
{
    viewRoom = new ViewRoom();
    connectViewRoom();
    viewRoom->editing = true;
    
    // Set the label and button to have the texts corresponding to edition
    viewRoom->lbl_title->setText(tr("Edition Salle"));
    viewRoom->btn_create->setText(tr("Editer"));
    
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
            // TO DO : Check that the user exists.
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
    // The room already exists so we check the user really wants to remove members
    // from it. We also must send a command to the server.
    if (viewRoom->editing)
    {
        // Stand in code.
        viewRoom->sim_members->removeRow(viewRoom->lst_members->currentIndex().row());
    }
    
    // The room isn't created yet, so we don't care if the members are removed and
    // it just need to be removed locally.
    else
    {
        viewRoom->sim_members->removeRow(viewRoom->lst_members->currentIndex().row());
    }
}

void ControllerRoom::removeMembers()
{
    // The room already exists so we check the user really wants to remove members
    // from it. We also must send a command to the server.
    if (viewRoom->editing)
    {
        // Stand in code.
        viewRoom->sim_members->clear();
        
    }
    
    // The room isn't created yet, so we don't care if the members are removed and
    // it just need to be removed locally.
    else
    {
        viewRoom->sim_members->clear();
    }
    
}

void ControllerRoom::createRoom()
{
    
    // The room must have a name.
    QString roomName = viewRoom->ldt_name->text().trimmed();
    if(roomName.isEmpty())
    {
        QMessageBox::information(viewRoom, tr("Opération impossible") ,tr("Vous devez entrer un nom pour la salle"));
        viewRoom->ldt_name->setFocus(Qt::MouseFocusReason);
        return;
    }
    
    // The room must have at least a member
    if(viewRoom->sim_members->rowCount() == 0)
    {
        QMessageBox::information(viewRoom, tr("Opération impossible") ,tr("Une salle doit posséder au moins un membre"));
        viewRoom->ldt_member->setFocus(Qt::MouseFocusReason);
        return;
    }
    
    // Here should be creation of room (not yet in interpretor).
}

void ControllerRoom::editRoom()
{
    
}

void ControllerRoom::cancelRoom()
{
    // Close and delete the viewRoom.
    viewRoom->close();
}

void ControllerRoom::cancelJoin()
{
    // Close and delete the viewJoin.
    viewJoin->close();
}

