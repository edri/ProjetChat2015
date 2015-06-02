/*
     * Created by Miguel Santamaria
     *
     * Implements controllerChat.h
*/

#include "controllerChat.h"

ControllerChat::ControllerChat(ModelChator* model, ModelUser* currentUser, ClientControllerInput* cci,
                               Interpretor* i, ClientConnector* cc, ControllerOutput* co, ControllerRoom* controllerRoom,
                               Cryptor* cryptor)
{
    _model = model;

    // Initialize the chat module's view.
    _view = new ViewChat(_model);
    _viewEdition = new ViewInscription(_view, currentUser);
    _viewRequests = new ViewMembershipRequests(_model);

    _currentUser = currentUser;
    _controllerRoom = controllerRoom;

    // Object used for the server communication.
    _cci = cci;
    _i = i;
    _cc = cc;
    _co = co;
    // Object used for the security.
    _cryptor = cryptor;

    connect(_view, SIGNAL(requestOpenRoomModule(const bool)), this, SLOT(openRoomModule(const bool)));
    connect(_view, SIGNAL(requestLoadRoomMessages(const quint32)), this, SLOT(loadRoomMessages(const quint32)));
    connect(_view, SIGNAL(requestSendMessage()), this, SLOT(sendMessage()));
    connect(_view, SIGNAL(requestEditMessage(const QTreeWidgetItem*)), this, SLOT(editMessage(const QTreeWidgetItem*)));
    connect(_view, SIGNAL(requestDeleteMessage(quint32, quint32)), this, SLOT(askServerToDeleteMessage(quint32, quint32)));
    connect(_view, SIGNAL(requestDeleteRoom(quint32)), this, SLOT(askServerToDeleteRoom(quint32)));
    connect(_view, SIGNAL(requestLeaveRoom(quint32)), this, SLOT(askServerToLeaveRoom(quint32)));
    connect(_view, SIGNAL(requestShowEditionView()), this, SLOT(showViewEdition()));
    connect(_view, SIGNAL(requestOpenRoomMembership()), this, SLOT(openRoomMembership()));
    connect(_view, SIGNAL(requestShowMembershipRequestsView()), this, SLOT(showMembershipRequestsView()));
    connect(_viewRequests, SIGNAL(requestProcessRequest(bool)), this, SLOT(processRequest(bool)));
    // Occurs when the serveur's connexion has been lost.
    connect(cc, SIGNAL(disconnected()), SLOT(serverDisconnected()));
    connect(_view, SIGNAL(requestCloseApplication()), this, SLOT(closeApplication()));
    // When a room windows has been closed we reactivate the chat view.
    connect(_controllerRoom, SIGNAL(reactivateChatWindows()), SLOT(reactivateWindows()));
}

ControllerChat::~ControllerChat()
{
    delete _view;
    delete _viewEdition;
    delete _viewRequests;
    delete _model;
    delete _currentUser;
    delete _cci;
    delete _i;
    delete _cc;
    delete _co;
    delete _controllerRoom;
    delete _cryptor;
}

void ControllerChat::showView() const
{
    // Give the current user's ID to the view, and show it.
    ViewChat::currentUserId = _currentUser->getIdUser();
    _view->setConnectedAsText(_currentUser->getUserName());
    _view->show();
}

void ControllerChat::loadUser(ModelUser& user) const
{
    qDebug() << "Réception d'un utilisateur";

    // Add the given user in the model.
    _model->addUser(user);
}

void ControllerChat::loadRoom(ModelRoom& room) const
{
    qDebug() << "Réception d'une salle";

    if (!_model->containsRoom(room.getIdRoom()))
    {
        // If we have a private room, we must decrypt its secret key with RSA,
        // and decrypt each of its messages with the secret key.
        if (room.isPrivate())
        {
            QString messageContent;

            // Decrypt the private room's secret key with the known RSA key pair.
            _cryptor->decryptWithRSA(room.getSecretKey(), _model->getRsaKeyPair());

            // Decrypt each message's content with the decrypted secret key, and
            // store it in binary format (UTF8).
            for (ModelMessage& message : room.getMessages())
            {
                CypherText cypher(message.getContent().size());
                memcpy(cypher.data(), message.getContent().data(), message.getContent().size());
                messageContent = QString::fromStdString(_cryptor->decryptWithAES(cypher, room.getSecretKey()));
                message.setContent(messageContent.toUtf8());
            }
        }

        // Add the given room to the model.
        _model->addRoom(room);
    }
}

void ControllerChat::editRoom(const ModelRoom& room)
{
    qDebug() << "Réception d'une salle éditée";

    // Edit the room in the model and then in the view.
    _model->modifyRoom(room.getIdRoom(), room.getName(), room.getLimit(),
                       room.isPrivate(), room.isVisible(), room.getPicture(), room.getAdmins(), room.getUsers());
    _view->modifyRoom(room.getIdRoom(), room.getName(), room.getPicture());
}

void ControllerChat::receiveMessage(ModelMessage& message, const bool edited) const
{
    QString messageContent;
    // Get the received message's room.
    ModelRoom& room = _model->getRoom(message.getIdRoom());

    // Decrypt the message if its room is private.
    if (room.isPrivate())
    {
        CypherText cypher(message.getContent().size());
        memcpy(cypher.data(), message.getContent().data(), message.getContent().size());
        messageContent = QString::fromStdString(_cryptor->decryptWithAES(cypher, room.getSecretKey()));
        message.setContent(messageContent.toUtf8());
    }

    // Add the message to the room, depending on its status.
    if (!edited)
        room.addMessage(message);
    else
        _model->modifyMessage(room.getIdRoom(), message.getIdMessage(), message.getContent(), message.getEditionDate());

    // Add/Edit the message in the view.
    _view->loadRoomMessage(message, edited);
}

void ControllerChat::userStatusChanged(const quint32 userId, const bool isConnected) const
{
    _view->userStatusChanged(userId, isConnected);
}

void ControllerChat::newMembershipRequest(const quint32 roomId, const ModelUser& user,
                                          const QByteArray& publicKey) const
{
    // Add the new membership request in the model.
    _model->addMembershipRequest(roomId, user, publicKey);
    // Add a new request in the views.
    _view->updateRequests(1);
    _viewRequests->refresh(_model->getRequests());
}

void ControllerChat::openRoomModule(const bool editRoom) const
{
    // Open the room's editing/adding windows.
    if (editRoom)
        _controllerRoom->showRoom(_view->getSelectedRoomId());
    else
        _controllerRoom->showRoom();
}

void ControllerChat::loadRoomMessages(const quint32 idRoom) const
{
    ModelRoom& room = _model->getRoom(idRoom);
    // Show/Hide the administration's buttons, depending on the connected user's
    // status.
    _view->updateButtons(room.getAdmins().contains(_currentUser->getIdUser()));
    // Load the room's messages in the view.
    _view->loadRoomMessages(_model->getRoom(idRoom).getMessages());
}

void ControllerChat::loadUserRooms() const
{
    // Get all the rooms of the connected user.
    QList<quint32> userRooms = _model->getUserRooms(_currentUser->getIdUser());

    // First of all, we clear the users of each room (because we maybe added
    // or removed some, since the last refresh).
    _view->clearRoomUsers();

    // Add each room in the view.
    for (quint32 roomId : userRooms)
    {
        ModelRoom& room = _model->getRoom(roomId);
        _view->addRoom(roomId, room.getName(), room.getPicture(), room.isPrivate());

        // Add each room's user in the view.
        for (quint32 userId : room.getUsers())
        {
            // Check if the room is private and if we have a request for the given user.
            // If it exists, we remove it, because it's useless.
            qint32 requestId = _model->searchRequest(roomId, userId);
            if (room.isPrivate() && requestId != -1)
            {
                // Remove the request from the model.
                _model->deleteRequest(requestId);
                // Remove the request from the requests' view.
                _viewRequests->removeRequest(requestId);
                // Remove a request from the main view.
                _view->updateRequests(-1);
            }

            ModelUser& user = _model->getUser(userId);
            _view->addUserToRoom(roomId, userId, user.getUserName(), user.getImage(), user.isConnected());
        }
    }

    _view->selectFirstRoom();
}

bool ControllerChat::isControllerActive() const
{
    // To know if the controller is currently active, we look at the main view's status.
    return !_view->isHidden();
}

void ControllerChat::sendMessage() const
{
    QByteArray messageContent;
    // Get the selected room.
    ModelRoom& room = _model->getRoom(_view->getSelectedRoomId());

    // If the room is private, we must encrypt the message's content with its secret key.
    if (room.isPrivate())
    {
        CypherText cypher(_cryptor->encryptWithAES(_view->getMessageText().toStdString(), room.getSecretKey()));
        messageContent = QByteArray((char*)cypher.data(), (int)cypher.size());
    }
    // Else we just need to convert the message's content to binary.
    else
    {
        messageContent = _view->getMessageText().toUtf8();
    }

    // Create a new ModelMessage object, and send it to the server.
    ModelMessage message(0, room.getIdRoom(), _currentUser->getIdUser(), QDateTime::currentDateTime(), QDateTime::currentDateTime(), messageContent);
    _co->sendMessage(message, false);
}

void ControllerChat::editMessage(const QTreeWidgetItem* item) const
{
    QByteArray messageContent;
    // Get the selected room.
    ModelRoom& room = _model->getRoom(_view->getSelectedRoomId());

    // If the room is private, we must encrypt the message's content with its secret key.
    if (room.isPrivate())
    {
        CypherText cypher(_cryptor->encryptWithAES(item->text(1).toStdString(), room.getSecretKey()));
        messageContent = QByteArray((char*)cypher.data(), (int)cypher.size());
    }
    // Else we just need to convert the message's content to binary.
    else
    {
        messageContent = item->text(1).toUtf8();
    }

    // Create a new ModelMessage object, and send it to the server.
    ModelMessage message(item->data(1, Qt::UserRole).toInt(), room.getIdRoom(), _currentUser->getIdUser(), QDateTime::currentDateTime(), QDateTime::currentDateTime(), messageContent);
    _co->sendMessage(message, true);
}

void ControllerChat::askServerToDeleteMessage(const quint32 roomId, const quint32 messageId) const
{
    _co->deleteMessage(roomId, messageId);
}

void ControllerChat::deleteMessageInModel(const quint32 roomId, const quint32 messageId) const
{
    // Remove the message of the model, and then delete it in the view.
    _model->deleteMessage(roomId, messageId);
    _view->deleteMessage(messageId);
}

void ControllerChat::askServerToDeleteRoom(const quint32 roomId) const
{
    // The connected user must be a room's administrator to delete it.
    if (_model->getRoom(roomId).getAdmins().contains(_currentUser->getIdUser()))
        _co->deleteRoom(roomId);
}

void ControllerChat::deleteRoomInModel(const quint32 roomId) const
{
    // Remove the room of the model, and then delete it in the view.
    _model->deleteRoom(roomId);
    _view->deleteRoom(roomId);
}

void ControllerChat::askServerToLeaveRoom(const quint32 roomId) const
{
    _co->leave(_currentUser->getIdUser(), roomId);
}

void ControllerChat::leaveRoomInModel(const quint32 userId, const quint32 roomId) const
{
    // Remove the link between the user and the room, in the model.
    _model->removeUser(userId, roomId);

    // Delete the room of the view, or remove the oser of the room, depending on who
    // asked to leave.
    if (userId == _currentUser->getIdUser())
    {
        _view->deleteRoom(roomId);
        _model->deleteRoom(roomId);
    }
    else
        _view->deleteUserFromRoom(userId, roomId);
}

void ControllerChat::showViewEdition()
{
    // Send the current user to the view.
    _viewEdition->setCurrentUser(_currentUser);

    // Open the inscription's window.
    _viewEdition->show();
    _viewEdition->setEnabled(true);
}

void ControllerChat::openRoomMembership()
{
    _controllerRoom->showJoin();
}

void ControllerChat::showMembershipRequestsView()
{
    // Refresh the membership requests and show the view.
    _viewRequests->refresh(_model->getRequests());
    _viewRequests->show();
}

void ControllerChat::processRequest(const bool accepted)
{
    // Get the selected membership request.
    ModelRequest request = _model->getRequest(_viewRequests->getSelectedRequestId());
    QByteArray aesKey;

    // If the user has been accepted in the room, we must send this room's secret key to the serveur.
    // To avoiding security issues, we encrypt this key with the requester-user public key.
    if (accepted)
    {
        AESKey roomSecretKey = request.getRoom().getSecretKey();

        // Prepare a RSA key in which we store the requester-user public key.
        // In other words, we convert the requester-user public key from QByteArray to RSAPair.
        RSAPair rsaKey;
        rsaKey.publicKey.resize(request.getPublicKey().size());
        memcpy(rsaKey.publicKey.data(), request.getPublicKey().data(), rsaKey.publicKey.size());
        // Encrypt the room's secret key with the requester-user public key (in RSAPair format).
        _cryptor->encryptWithRSA(roomSecretKey, rsaKey);

        // Put the encrypted room's secret key in a QByteArray, for sending it to the server.
        QDataStream stream(&aesKey, QIODevice::WriteOnly);
        stream << QByteArray((char*)roomSecretKey.key.data(), (int)roomSecretKey.key.size()) << QByteArray((char*)roomSecretKey.initializationVector.data(), (int)roomSecretKey.initializationVector.size());
    }

    // Send to the server the request status.
    _co->changeRequestStatus(request.getRoom().getIdRoom(), request.getUser(), aesKey, accepted);
    // Remove the request from the model.
    _model->deleteRequest(request.getId());
    // Remove the request from the requests' view.
    _viewRequests->removeRequest();
    // Remove a request from the main view.
    _view->updateRequests(-1);
}

void ControllerChat::serverDisconnected()
{
    // If the view is currently opened, we ask it to show a disconnection's message.
    if (!_view->isHidden())
        _view->serverDisconnected();
}

void ControllerChat::reactivateWindows()
{
    _view->setEnabled(true);
}

void ControllerChat::closeApplication()
{
    exit(0);
}

ViewInscription* ControllerChat::getViewEdition()
{
    return _viewEdition;
}
