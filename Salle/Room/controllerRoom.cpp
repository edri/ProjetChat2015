/*
 * File : controllerRoom.cpp
 * Project : ProjetChat2015
 * Author(s) : Jan Purro
 * Last Modified : 25.04.2015 12:00
 * Description : Controller for the room module.
 * 
 * The room module manage chat room's creation and edition(management) as well 
 * as user joining a room.
 * Two views are available in this module : one for creation/edition and one for
 * joining called respectively ViewRoom and ViewJoin.
 * The model is the application model (ModelChator).
 * 
 * The controller will open views when needed, retriew and insert data from/in 
 * the model and take care of the communications with the server for this
 * module.
 */


#include "controllerRoom.h"
#include <iostream>

ControllerRoom::ControllerRoom(ModelChator* model, ModelUser* user)
{
    this->model = model;
    currentUser = user;
    
    // Initialize the view at nullptr. They will be constructed when needed
    // and destroyed when closed.
    viewRoom = nullptr;
    viewJoin = nullptr;
}

void ControllerRoom::connectViewRoom()
{
    // Connect the view signals with the controller's slots.
    connect(viewRoom, SIGNAL(add()), this, SLOT(addUser()));
    connect(viewRoom, SIGNAL(remove(quint32)), this, SLOT(removeUser(quint32)));
    connect(viewRoom, SIGNAL(cancel()), this, SLOT(cancelRoom()));
    connect(viewRoom, SIGNAL(create()), this, SLOT(actionRoom()));
    connect(viewRoom, SIGNAL(admin()), this, SLOT(toggleAdmin()));
}

void ControllerRoom::connectViewJoin()
{
    // Connect the view signals with the controller's slots.
    connect(viewJoin->btn_join, SIGNAL(clicked()), this, SLOT(joinRoom()));
    connect(viewJoin->btn_cancel, SIGNAL(clicked()), this, SLOT(cancelJoin()));
}

void ControllerRoom::actionRoom()
{
    // If the room is currently editing an already existing room, call the 
    // function for editing rooms.
    // Otherwise a new room is created.
    if(viewRoom->isEditing())
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
    // Connect the view's signals
    connectViewRoom();
    
    // Set the label and button to have the texts corresponding to creation
    viewRoom->setTitle(tr("Nouvelle Salle"));
    viewRoom->setAction(tr("Créer"));
    viewRoom->setRemove(tr("Enlever"));
    
    // Add the user as a member and admin of the room.
    viewRoom->addUser(currentUser->getIdUser(), currentUser->getUserName(), true);
    // Open the the view window.
    viewRoom->show();
}

void ControllerRoom::showRoom(const quint32 idRoom)
{
    viewRoom = new ViewRoom();
    // Connect the view signals
    connectViewRoom();
    viewRoom->setEditing(true);
    
    // Retrieve the room from the id
    ModelRoom room = model->getRoom(idRoom);
    // Set the label and button to have the texts corresponding to edition
    viewRoom->setTitle(tr("Edition Salle"));
    viewRoom->setAction(tr("Editer"));
    viewRoom->setRemove(tr("Bannir"));
    
    // Initialize the fields with the values of the room.
    viewRoom->setRoomName(room.getName());
    viewRoom->setNbMessage(room.getLimit());
    //viewRoom->setRoomLogo(room.getPicture()); // How to retrieve the path for this image ?
    viewRoom->setRoomLogo("image.png"); // Stand in text
    
    // Set the room visibility and privacy.
    viewRoom->setPrivate(room.isPrivate());
    if(room.isPrivate())
    {
        viewRoom->setRoomVisibility(room.isVisible());
        viewRoom->setInvitation(!room.isVisible());
    }
    
    // Load the users and admins.
    ModelUser user;
    for(quint32 userId : room.getUsers())
    {
        user = model->getUser(userId);
        viewRoom->addUser(user.getIdUser(), user.getUserName());
    }
    
    ModelUser admin;
    for(quint32 adminId : room.getAdmins())
    {
        admin = model->getUser(adminId);
        viewRoom->addUser(admin.getIdUser(), admin.getUserName(), true);
    }
    // Open the the view window.
    viewRoom->show();
}

void ControllerRoom::addUser()
{
    // Get the name entered by the user in the view.
    QString userName = viewRoom->userName();
    
    // Ignore is the user name is empty.
    if(!userName.isEmpty())
    {
        // Disable the room while waiting for the server answer.
        viewRoom->setDisabled(true);
        
        // Ask the server for the user id.
        
        // For now call the function itself
        for(long long unsigned i = 0; i < 100000000; ++i) // Emulates waiting 
        {
            std::cout << "";
        }
        
        if (userName == "Dieu") // Simulate non-existent user.
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
    // Enable the room again.
    viewRoom->setDisabled(false);
    // Add the user.
    viewRoom->addUser(userId, userName);
}

void ControllerRoom::userDoesNotExist()
{
    // Enable the room again
    viewRoom->setDisabled(false);
    // Display an information box.
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
    // First run a few checks and inform the users if something is wrong/missing.
    
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
    
    // The room logo must be either empty or a valid image.
    QString logoPath = viewRoom->roomLogo();
    // If the logoPath is empty or not an image, a null image will be 
    // created. We check for null image only in the case the path was'nt empty.
    QImage roomLogo(logoPath);
    // Check for invalid path, should the format/size be checked too ?
    if(!logoPath.isEmpty() && roomLogo.isNull())
    {
        QMessageBox::information(viewRoom, tr("Opération impossible") ,tr("Le fichier du logo n'est pas une image valide. Si le nom du fichier commence ou se termine par une espace, essayer de la supprimer."));
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
