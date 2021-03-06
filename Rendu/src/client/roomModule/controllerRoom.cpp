/*
 * File : controllerRoom.cpp
 * Project : ProjetChat2015
 * Author(s) : Jan Purro
 * Last Modified : 25.04.2015 14:25 by Jan Purro
 * Description : Implementation room module controller (see controllerRoom.h for
 * more info).
 */
#include "../../common/models/chatorConstants.h"
#include "controllerRoom.h"

ControllerRoom::ControllerRoom(ModelChator* model, ModelUser* user, ControllerOutput* controllerOuput, Cryptor* cryptor)
{
    _model = model;
    _currentUser = user;
    _controllerOutput = controllerOuput;
    _cryptor = cryptor;
    _currentRoomId = 0;
    
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
    connect(_viewRoom, SIGNAL(reactivateChatWindows()), this, SLOT(askReactivateChatWindows()));
}

void ControllerRoom::connectViewJoin()
{
    // Connect the view signals with the controller's slots.
    connect(_viewJoin, SIGNAL(join(quint32, bool)), this, SLOT(joinRoom(quint32, bool)));
    connect(_viewJoin, SIGNAL(cancel()), this, SLOT(cancelJoin()));
    connect(_viewJoin, SIGNAL(reactivateChatWindows()), this, SLOT(askReactivateChatWindows()));
}

void ControllerRoom::showRoom()
{
    _viewRoom = new ViewRoom();
    _currentRoomId = 0;
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
    _currentRoomId = idRoom;
    // Connect the view signals
    connectViewRoom();
    
    // Retrieve the room from the id
    ModelRoom room = _model->getRoom(idRoom);
    // Set the label and button to have the texts corresponding to edition
    _viewRoom->setTitle(tr("Edition Salle"));
    _viewRoom->setAction(tr("Editer"));
    _viewRoom->setRemove(tr("Enlever")); // Should be ban, but it isn't implemented yet.
    
    // Initialize the fields with the values of the room.
    _viewRoom->setRoomName(room.getName());
    _viewRoom->setNbMessage(room.getLimit());
    //viewRoom->setRoomLogo(room.getPicture()); // How to retrieve the path for this image ?
    //_viewRoom->setRoomLogo("image.png"); // Stand in text
    
    // Set the room visibility and privacy.
    _viewRoom->setPrivate(room.isPrivate());
    if(room.isPrivate())
    {
        _viewRoom->setRoomVisibility(room.isVisible());
        _viewRoom->setInvitation(!room.isVisible());
    }
    
    // Disable the possibility to change the privacy and visibility of the room.
    _viewRoom->disablePrivacy();
    
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
    
    // Ignore if the user name is empty.
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
    QList<QPair<quint32, QByteArray>> idsAndKeys;
    createRoom(idsAndKeys);
}

void ControllerRoom::createRoom(QList<QPair<quint32, QByteArray>>& idsAndKeys)
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
    
    qDebug() << "Je suis en train de créer / modifier une salle";
    
    /* If a path was given for the logo, load the image situated at the path.
     * Else if the room is being edited reuse the old logo. If this is a new room
     * and no path is given an empty image will be sent.*/
    QImage logo;
    if (!_viewRoom->roomLogo().isEmpty())
    {
        logo = QImage(_viewRoom->roomLogo()).scaled(PICTURE_SIZE, PICTURE_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    
    else if (_viewRoom->roomLogo().isEmpty() && _viewRoom->isEditing())
    {
        logo = _model->getRoom(_currentRoomId).getPicture();
    }
    
    AESKey roomKey;
    QMap<quint32, QByteArray> usersAndKeys;
    // If the room is private, the key must be encrypted by the public key of each added memebers.
    // We need those key and demands them to the server if they are not given.
    // For public rooms, empty keys are created and added to the list.
    if(_viewRoom->isRoomPrivate() && idsAndKeys.isEmpty())
    {
        QByteArray emptyKey;
        QPair<quint32, QByteArray> idAndKey;
        idAndKey.second = emptyKey;
        // Ask the keys of all the members of the room.
        for(quint32 id : _viewRoom->roomUsers())
        {
           idAndKey.first = id;
           idsAndKeys.append(idAndKey);
        }
        _controllerOutput->publicKey(idsAndKeys);
         return;
    }
    
    else if (_viewRoom->isRoomPrivate())
    {
        // If the room is beign edited its key is retreived from the model.
        // Otherwise a new key is generated.
        if(_currentRoomId)
        {
            roomKey = _model->getRoom(_currentRoomId).getSecretKey();
        }
        
        else
        {
            roomKey = _cryptor->generateAESKey();
        }
        
        AESKey cryptedKey;
        RSAPair rsaKeys;
        
        // For each member of the room, the room's secret key is encrypted with 
        // their respective public key.
        for(QPair<quint32, QByteArray> pair : idsAndKeys)
        {
            QByteArray aesKey;
            QDataStream stream(&aesKey, QIODevice::WriteOnly);
            
            cryptedKey = roomKey;
            // We copy the given ByteArray into an RSAPair (we only need the 
            // public key field.
            rsaKeys.publicKey.resize(pair.second.size());
            memcpy(rsaKeys.publicKey.data(), pair.second.data(), rsaKeys.publicKey.size());
            
            _cryptor->encryptWithRSA(cryptedKey, rsaKeys);
            
            // Write the encrypted key and IV into a byte array.
            stream << QByteArray((char*)cryptedKey.key.data(), (int)cryptedKey.key.size()) << QByteArray((char*)cryptedKey.initializationVector.data(), (int)cryptedKey.initializationVector.size());
            usersAndKeys.insert(pair.first, aesKey);
        
        }
    }
    
    else
    {
        for(quint32 id : _viewRoom->roomUsers())
        {
           usersAndKeys.insert(id, QByteArray());
        }
    }
    
    // Retreive the messages if the room is being edited. Otherwise use an empty
    // list of message
    QMap<quint32, ModelMessage> messages;
    if(_viewRoom->isEditing())
    {
        messages = _model->getRoom(_currentRoomId).getMessages();
    }
    
    // The key in the ModelRoom is left empty (it will be different for each user, who will receive them encrypted.
    roomKey = AESKey();
    
    // Create the model room
    ModelRoom newRoom(_currentRoomId, _viewRoom->roomName(), _viewRoom->messageLimit(), _viewRoom->isRoomPrivate(), _viewRoom->isRoomVisible(), logo, _viewRoom->roomAdmins(), _viewRoom->roomUsers(), messages, roomKey);
    
    // Send the packet to the server.
    _controllerOutput->room(newRoom, usersAndKeys, _viewRoom->isEditing());
    
    // Close the view.
    _viewRoom->close();
}

bool ControllerRoom::isValidImage(const QString& path)
{
    // An empty path will crate an empty image.
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
    createRoom();
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

void ControllerRoom::askReactivateChatWindows()
{
    emit reactivateChatWindows();
}

void ControllerRoom::joinRoom(quint32 roomId, bool isPrivate)
{
   // Inform the server that the user wish to join a room.
   _controllerOutput->joinRoom(roomId);
   // Display an informative box message if the room is private.
   if(isPrivate)
   {
       QMessageBox::information(_viewRoom, tr("Salle Privée") ,tr("Une demande d'adhésion a été envoyée."));
   }
   _viewJoin->close();
}

void ControllerRoom::userId(bool exists, quint32 userId)
{
    if(!exists)
    {   
        // If the user exist display an informative popup
        QMessageBox::information(_viewRoom, tr("Opération impossible") ,tr("Cet utilisateur n'existe pas."));
    }
    
    else
    {
        // Add the user.
        _viewRoom->addUser(userId, _viewRoom->userName());
    }
    
    // Enable the view again
    _viewRoom->setDisabled(false);
    _viewRoom->focusAddUser();
}

void ControllerRoom::showJoin()
{
    _controllerOutput->askForRooms();
    _viewJoin = new ViewJoin();
    _viewJoin->show();
}

void ControllerRoom::listRooms(const QList<QPair<quint32, QString>>& publicRooms,
                               const QList<QPair<quint32, QString>>& privateRooms)
{
    // Removes the public rooms the user is already a member of.
    QList<QPair<quint32, QString>> roomsList;
    for(QPair<quint32, QString> pair : publicRooms)
    {
        if(! _model->containsRoom(pair.first))
        {
            roomsList.append(pair);
        }
    }
    
    _viewJoin->setPublicRooms(roomsList);
    
    roomsList.clear();
    
    // Removes the private rooms the user is already a member of.
    for(QPair<quint32, QString> pair : privateRooms)
    {
        if(! _model->containsRoom(pair.first))
        {
            roomsList.append(pair);
        }
    }
    _viewJoin->setPrivateRooms(roomsList);
      
    _viewJoin->loadRooms();
    connectViewJoin();
}

void ControllerRoom::closeWindows()
{
    _viewJoin->close();
    _viewRoom->close();
}
