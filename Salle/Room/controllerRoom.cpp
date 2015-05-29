/*
 * File : controllerRoom.cpp
 * Project : ProjetChat2015
 * Author(s) : Jan Purro
 * Last Modified : 25.04.2015 14:25 by Jan Purro
 * Description : Implementation room module controller (see controllerRoom.h for
 * more info).
 */

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
    connect(_viewRoom, SIGNAL(edit()), this, SLOT(editRoom()));
}

void ControllerRoom::connectViewJoin()
{
    // Connect the view signals with the controller's slots.
    connect(_viewJoin, SIGNAL(join(quint32)), this, SLOT(joinRoom(quint32)));
    connect(_viewJoin, SIGNAL(cancel()), this, SLOT(cancelJoin()));
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
    _viewRoom->setRoomLogo("image.png"); // Stand in text
    
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
    
    // Construit l'image
    QImage logo;
    if (!_viewRoom->roomLogo().isEmpty())
    {
        logo.load(_viewRoom->roomLogo());
    }
    
    AESKey roomKey;
    QList<QPair<QByteArray, QByteArray>> cryptedKeys;
    QList<quint32> usersIds;
    // If the room is private, the key must be encrypted by the public key of each added memebers.
    // We need those key and demands them to the server.
    if(_viewRoom->isRoomPrivate() && idsAndKeys.isEmpty())
    {
        QByteArray emptyKey;
        QPair<quint32, QByteArray> idAndKey;
        idAndKey.second = emptyKey;
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
        if(_currentRoomId)
        {
            roomKey = _model->getRoom(_currentRoomId).getSecretKey();
        }
        
        else
        {
            roomKey = _cryptor->generateAESKey();
            QByteArray tmp((const char*) roomKey.key.data(), roomKey.key.size());
            qDebug() << "Clé secrète : " << QString::fromUtf8(tmp.toHex());
        }
        AESKey cryptedKey;
        
        QPair<QByteArray, QByteArray> newPair;
        RSAPair rsaKeys;
        for(QPair<quint32, QByteArray> pair : idsAndKeys)
        {
            cryptedKey = roomKey;
            usersIds.append(pair.first);
            rsaKeys.publicKey.resize(pair.second.size());
            memcpy(rsaKeys.publicKey.data(), pair.second.data(), rsaKeys.publicKey.size());
            
            QByteArray tmp3((const char*) rsaKeys.publicKey.data(), rsaKeys.publicKey.size());
            qDebug() << "Clé public : " << QString::fromUtf8(tmp3.toHex());
            
            _cryptor->encryptWithRSA(cryptedKey, rsaKeys);
            
            QByteArray tmp2((const char*) cryptedKey.key.data(), cryptedKey.key.size());
            qDebug() << "Clé secrète chiffrée : " << QString::fromUtf8(tmp2.toHex());
            
            newPair.first = QByteArray((char*)cryptedKey.key.data(), cryptedKey.key.size());
            newPair.second = QByteArray((char*)cryptedKey.initializationVector.data(), cryptedKey.initializationVector.size());
            cryptedKeys.append(newPair);
        
        }
        //_cryptor->encryptWithRSA(roomKey, _model->getRsaKeyPair());
    }
    
    else
    {
        for(quint32 id : _viewRoom->roomUsers())
        {
           usersIds.append(id);
        }
    }
    
    // Construct a ModelRoom object.
    QMap<quint32, ModelMessage> messages;
    roomKey = AESKey();
    ModelRoom newRoom(_currentRoomId, _viewRoom->roomName(), _viewRoom->messageLimit(), _viewRoom->isRoomPrivate(), _viewRoom->isRoomVisible(), logo, _viewRoom->roomAdmins(), _viewRoom->roomUsers(), messages, roomKey);
    
    _controllerOutput->room(newRoom, usersIds, cryptedKeys, _viewRoom->isEditing());
    
    //_viewRoom->setDisabled(false);
    // Faut que qqun ferme cette fenêtre à ce moment...
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

void ControllerRoom::joinRoom(quint32 roomId)
{
   // Inform the server that the user wish to join a room.
   _controllerOutput->joinRoom(roomId);
   _viewJoin->close();
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

void ControllerRoom::showJoin()
{
    _controllerOutput->askForRooms();
    _viewJoin = new ViewJoin();
    _viewJoin->show();
}

void ControllerRoom::listRooms(const QList<QPair<quint32, QString>>& publicRooms,
                               const QList<QPair<quint32, QString>>& privateRooms)
{
    _viewJoin->setPublicRooms(publicRooms);
    _viewJoin->setPrivateRooms(privateRooms);
}
