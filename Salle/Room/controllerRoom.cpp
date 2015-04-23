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
    connect(viewRoom, SIGNAL(add()), this, SLOT(addUser()));
    connect(viewRoom, SIGNAL(remove(quint32)), this, SLOT(removeUser(quint32)));
    connect(viewRoom, SIGNAL(cancel()), this, SLOT(cancelRoom()));
    connect(viewRoom, SIGNAL(create()), this, SLOT(actionRoom()));
    connect(viewRoom, SIGNAL(admin()), this, SLOT(toggleAdmin()));
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
    connectViewRoom();
    
    // Set the label and button to have the texts corresponding to creation
    viewRoom->setTitle(tr("Nouvelle Salle"));
    viewRoom->setAction(tr("Créer"));
    viewRoom->setRemove(tr("Enlever"));
    
    // Add the user as a member of the room.
    viewRoom->addUser(user->getIdUser(), user->getUserName(), true);
    viewRoom->show();
}

void ControllerRoom::showRoom(const quint32 idRoom)
{
    viewRoom = new ViewRoom();
    connectViewRoom();
    viewRoom->editing = true;
    
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
        viewRoom->setRoomVisibility(room->isVisible());
        viewRoom->setInvitation(!room->isVisible());
    }
    
    for(ModelUser* user : room->getUsers())
    {
        viewRoom->addUser(user->getIdUser(), user->getUserName());
    }
    
    for(ModelUser* user : room->getAdmins())
    {
        viewRoom->addUser(user->getIdUser(), user->getUserName(), true);
    }
    viewRoom->show();
}

void ControllerRoom::addUser()
{
    QString userName = viewRoom->userName();
    
    // Ignore is the user name is empty.
    if(!userName.isEmpty())
    {
        viewRoom->setDisabled(true);
        
        // Ask the server for the user id.
        // For now call the function itself
        for(long long unsigned i = 0; i < 100000000; ++i)
        {
            std::cout << "";
        }
        if (userName == "Dieu")
        {
            userDoesNotExist();
        }
        else
        {
            addUser(0, userName);
        }
    }
}

void ControllerRoom::addUser(const quint32 userId, const QString& userName)
{
    viewRoom->setDisabled(false);
    viewRoom->addUser(userId, userName);
}

void ControllerRoom::userDoesNotExist()
{
    viewRoom->setDisabled(false);
    QMessageBox::information(viewRoom, tr("Opération impossible") ,tr("Cette utilisateur n'existe pas."));
}

void ControllerRoom::removeUser(const quint32 userId)
{
    if (viewRoom->editing)
    {
        // Ban the user
    }
    
    else
    {
        viewRoom->removeUser(userId);
    }
}

void ControllerRoom::createRoom()
{
    // The room must have a name.
    if(viewRoom->roomName().isEmpty())
    {
        QMessageBox::information(viewRoom, tr("Opération impossible") ,tr("Vous devez entrer un nom pour la salle"));
        //viewRoom->ldt_name->setFocus(Qt::MouseFocusReason);
        return;
    }
    
    // The room must have at least a member
    if(viewRoom->roomUsers().isEmpty())// sim_members->rowCount() == 0)
    {
        QMessageBox::information(viewRoom, tr("Opération impossible") ,tr("Une salle doit posséder au moins un membre"));
        //viewRoom->ldt_member->setFocus(Qt::MouseFocusReason);
        return;
    }
    
    // The room must have at least an admin.
    if(viewRoom->roomAdmins().isEmpty())
    {
        QMessageBox::information(viewRoom, tr("Opération impossible") ,tr("Une salle doit posséder au moins un admin"));
        //viewRoom->ldt_member->setFocus(Qt::MouseFocusReason);
        return;
    }
    
    // As soon as interpretor allows to create a new room, will create a new room.
}

void ControllerRoom::editRoom()
{
    
}

void ControllerRoom::toggleAdmin()
{
    if (viewRoom->editing)
    {
        // Communicate with the server (not sure if it should).
    }
    
    viewRoom->toggleAdmin();
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

void ControllerRoom::joinRoom()
{
   
}
