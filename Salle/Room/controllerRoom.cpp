/*
 * File : controllerRoom.cpp
 * Project : ProjetChat2015
 * Author(s) : Jan Purro
 * Last Modified : 25.04.2015 14:25 by Jan Purro
 * Description : Implementation room module controller (see controllerRoom.h for
 * more info).
 */

#include "controllerRoom.h"
#include <iostream>

ControllerRoom::ControllerRoom(ModelChator* model, ModelUser* user, ControllerOutput* controllerOuput, Cryptor* cryptor)
{
    _model = model;
    _currentUser = user;
    _controllerOutput = controllerOuput;
    _cryptor = cryptor;
    
    // Initialize the view at nullptr. They will be constructed when needed
    // and destroyed when closed.
    _viewRoom = nullptr;
    _viewJoin = nullptr;
}

void ControllerRoom::connectViewRoom()
{
    // Connect the view signals with the controller's slots.
    connect(_viewRoom, SIGNAL(add()), this, SLOT(addUser()));
    connect(_viewRoom, SIGNAL(cancel()), this, SLOT(cancelRoom()));
    connect(_viewRoom, SIGNAL(create()), this, SLOT(createRoom()));
    connect(_viewRoom, SIGNAL(edit()), this, SLOT(editRoom()));
}

void ControllerRoom::connectViewJoin()
{
    // Connect the view signals with the controller's slots.
    connect(_viewJoin, SIGNAL(join()), this, SLOT(joinRoom()));
    connect(_viewJoin, SIGNAL(cancel), this, SLOT(cancelJoin()));
}

void ControllerRoom::showRoom()
{
    _viewRoom = new ViewRoom();
    // Connect the view's signals
    connectViewRoom();
    
    // Set the label and button to have the texts corresponding to creation
    _viewRoom->setTitle(tr("Nouvelle Salle"));
    _viewRoom->setAction(tr("Créer"));
    _viewRoom->setRemove(tr("Enlever"));
    
    // Add the user as a member and admin of the room.
    _viewRoom->addUser(_currentUser->getIdUser(), _currentUser->getUserName(), true);
    // Open the the view window.
    _viewRoom->show();
}

void ControllerRoom::showRoom(const quint32 idRoom)
{
    _viewRoom = new ViewRoom(true);
    // Connect the view signals
    connectViewRoom();
    
    // Retrieve the room from the id
    ModelRoom room = _model->getRoom(idRoom);
    // Set the label and button to have the texts corresponding to edition
    _viewRoom->setTitle(tr("Edition Salle"));
    _viewRoom->setAction(tr("Editer"));
    _viewRoom->setRemove(tr("Bannir"));
    
    // Initialize the fields with the values of the room.
    _viewRoom->setRoomName(room.getName());
    _viewRoom->setNbMessage(room.getLimit());
    //viewRoom->setRoomLogo(room.getPicture()); // How to retrieve the path for this image ?
    _viewRoom->setRoomLogo("image.png"); // Stand in text
    
    // Set the room visibility and privacy.
    _viewRoom->setPrivate(room.isPrivate());
    if(room.isPrivate())
    {
        _viewRoom->setRoomVisibility(room.isVisible());
        _viewRoom->setInvitation(!room.isVisible());
    }
    
    // Load the users and admins.
    ModelUser user;
    for(quint32 userId : room.getUsers())
    {
        user = _model->getUser(userId);
        _viewRoom->addUser(user.getIdUser(), user.getUserName());
    }
    
    ModelUser admin;
    for(quint32 adminId : room.getAdmins())
    {
        admin = _model->getUser(adminId);
        _viewRoom->addUser(admin.getIdUser(), admin.getUserName(), true);
    }
    // Open the the view window.
    _viewRoom->show();
}

void ControllerRoom::addUser()
{
    // Get the name entered by the user in the view.
    QString userName = _viewRoom->userName();
    
    // Ignore is the user name is empty.
    if(!userName.isEmpty())
    {
        // Disable the room while waiting for the server answer.
        _viewRoom->setDisabled(true);
        
        // Ask the server for the user id.
        _controllerOutput->userId(userName);
    }
}

void ControllerRoom::createRoom()
{
    // Disable the view again
    _viewRoom->setDisabled(true);
    
    // First run a few checks and inform the users if something is wrong/missing.
    
    // The room must have a name.
    if(_viewRoom->roomName().isEmpty())
    {
        QMessageBox::information(_viewRoom, tr("Opération impossible") ,tr("Vous devez entrer un nom pour la salle"));
        //viewRoom->ldt_name->setFocus(Qt::MouseFocusReason);
        
        // Enable the view again
        _viewRoom->setDisabled(false);
        return;
    }
    
    // The room must have at least a member
    if(_viewRoom->roomUsers().isEmpty())// sim_members->rowCount() == 0)
    {
        QMessageBox::information(_viewRoom, tr("Opération impossible") ,tr("Une salle doit posséder au moins un membre"));
        //viewRoom->ldt_member->setFocus(Qt::MouseFocusReason);
        // Enable the view again
        _viewRoom->setDisabled(false);
        return;
    }
    
    // The room must have at least an admin.
    if(_viewRoom->roomAdmins().isEmpty())
    {
        QMessageBox::information(_viewRoom, tr("Opération impossible") ,tr("Une salle doit posséder au moins un admin"));
        //viewRoom->ldt_member->setFocus(Qt::MouseFocusReason);
        // Enable the view again
        _viewRoom->setDisabled(false);
        return;
    }
    
    // The room logo must be either empty or a valid image.
    if(!isValidImage(_viewRoom->roomLogo()))
    {
        QMessageBox::information(_viewRoom, tr("Opération impossible") ,tr("Le fichier du logo n'est pas une image valide. Si le nom du fichier commence ou se termine par une espace, essayer de la supprimer."));
        // Enable the view again
        _viewRoom->setDisabled(false);
        return;
    }
    
    // Construit l'image
    QImage logo;
    if (!_viewRoom->roomName().isEmpty())
    {
        logo.load(_viewRoom->roomName());
    }
    
    // Construct a ModelRoom object.
    QMap<quint32, ModelMessage> messages;
    ModelRoom newRoom(0, _viewRoom->roomName(), _viewRoom->messageLimit(), _viewRoom->isRoomPrivate(), _viewRoom->isRoomVisible(), logo, _viewRoom->roomAdmins(), _viewRoom->roomUsers(), messages);
    
    _controllerOutput->room(newRoom);
    
    _viewRoom->setDisabled(false);
}

bool ControllerRoom::isValidImage(const QString& path)
{
    if (path.isEmpty())
    {
        return true;
    }
    // Check for invalid path or image, should the format/size be checked too ?
    QImage logo(path);
    
    return !logo.isNull();
}

void ControllerRoom::editRoom()
{
    // Edit the room. This means validating the changes and forwarding them to the server.
}

void ControllerRoom::cancelRoom()
{
    // Close and delete the viewRoom (the viewRoom is set to be destroyed when closed).
    _viewRoom->close();
}

void ControllerRoom::cancelJoin()
{
    // Close and delete the viewJoin (the viewJoin is set to be destroyed when closed).
    _viewJoin->close();
}

void ControllerRoom::joinRoom()
{
   // Inform the server that the user wish to join a room.
}

void ControllerRoom::userId(bool exists, quint32 userId)
{
    if(!exists)
    {   
        QMessageBox::information(_viewRoom, tr("Opération impossible") ,tr("Cette utilisateur n'existe pas."));
    }
    
    else
    {
        // Add the user.
        _viewRoom->addUser(userId, _viewRoom->userName());
    }
    
    // Enable the view again
    _viewRoom->setDisabled(false);
    
}
