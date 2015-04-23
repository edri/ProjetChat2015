#include "controllerRoom.h"
#include <iostream>

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
    connect(viewRoom, SIGNAL(add()), this, SLOT(addMember()));
    connect(viewRoom, SIGNAL(remove()), this, SLOT(removeMember()));
    connect(viewRoom, SIGNAL(cancel()), this, SLOT(cancelRoom()));
    connect(viewRoom, SIGNAL(create()), this, SLOT(actionRoom()));
}

void ControllerRoom::connectViewJoin()
{
    connect(viewJoin->btn_join, SIGNAL(clicked()), this, SLOT(joinRoom()));
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
    //connectViewRoom();
    viewRoom->show();
    return;
    
    // Set the label and button to have the texts corresponding to creation
    viewRoom->setTitle(tr("Nouvelle Salle"));
    viewRoom->setAction(tr("Créer"));
    viewRoom->setRemove(tr("Enlever"));
    
    // Add the user as a member of the room.
    viewRoom->addMember(user->getUserName());
    
    viewRoom->show();
}

void ControllerRoom::showRoom(const quint32 idRoom)
{
    viewRoom = new ViewRoom();
    //connectViewRoom();
    viewRoom->editing = true;
    viewRoom->show();
    return;
    // Retrieve the room from the id
    const ModelRoom* room = model->getRoom(idRoom);
    // Set the label and button to have the texts corresponding to edition
    viewRoom->setTitle(tr("Edition Salle"));
    viewRoom->setAction(tr("Editer"));
    viewRoom->setRemove(tr("Bannir"));
    
    // Initialize the fields with the values of the room.
    viewRoom->setRoomName(room->getName());
    viewRoom->setNbMessage(room->getLimit());
    viewRoom->setRoomLogo(room->getPicture());
    viewRoom->setPrivate(room->isPrivate());
    if(room->isPrivate())
    {
        viewRoom->setVisible(room->isVisible());
        viewRoom->setInvitation(!room->isVisible());
    }
    viewRoom->loadMembers(room);
    viewRoom->show();
}

void ControllerRoom::addMember()
{
    viewRoom->addMember();
}

void ControllerRoom::removeMember()
{
    viewRoom->removeMember();
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
    if(viewRoom->lsw_members->model()->rowCount() == 0)
    {
        QMessageBox::information(viewRoom, tr("Opération impossible") ,tr("Une salle doit posséder au moins un membre"));
        viewRoom->ldt_member->setFocus(Qt::MouseFocusReason);
        return;
    }
    
    //(const quint32 idRoom, const QString& name, const quint32 limitOfStoredMessage, const bool isPrivate,
    //      const bool isVisible, const QString& picture, QMap<quint32, ModelUser*>& admins, QMap<quint32, ModelUser*>& users) 
}

void ControllerRoom::editRoom()
{
    
}


void ControllerRoom::joinRoom()
{
   /*
    *currentStaff = list_staff->selectionModel()->currentIndex();
    
    if(!currentStaff->isValid())
    {
        return;
    }
    
    Item* staff = (Item*) model_staff->itemFromIndex(*currentStaff);
    */
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

