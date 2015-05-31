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
    _view = new ViewChat(_model);
    _viewEdition = new ViewInscription(_view, currentUser);
    _viewRequests = new ViewMembershipRequests(_model);

    _currentUser = currentUser;
    _cci = cci;
    _i = i;
    _cc = cc;
    _co = co;
    _controllerRoom = controllerRoom;
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
}

ControllerChat::~ControllerChat()
{
    _controllerRoom->closeWindows();
    delete _view;
    delete _model;
    delete _viewEdition;
}

void ControllerChat::showView() const
{
    ViewChat::currentUserId = _currentUser->getIdUser();
    _view->setConnectedAsText(_currentUser->getUserName());
    _view->show();
}

void ControllerChat::loadUser(ModelUser& user) const
{
    _model->addUser(user);
}

void ControllerChat::loadRoom(ModelRoom& room) const
{
    qDebug() << "Réception d'une salle";
    if (room.isPrivate())
    {
        QString messageContent;

        _cryptor->decryptWithRSA(room.getSecretKey(), _model->getRsaKeyPair());

        for (ModelMessage& message : room.getMessages())
        {
            CypherText cypher(message.getContent().size());
            memcpy(cypher.data(), message.getContent().data(), message.getContent().size());
            messageContent = QString::fromStdString(_cryptor->decryptWithAES(cypher, room.getSecretKey()));
            message.setContent(messageContent.toUtf8());
        }
    }

    _model->addRoom(room);
}

void ControllerChat::editRoom(const ModelRoom& room)
{
    _model->modifyRoom(room.getIdRoom(), room.getName(), room.getLimit(),
                       room.isPrivate(), room.isVisible(), room.getPicture());
    _view->modifyRoom(room.getIdRoom(), room.getName(), room.getPicture());
}

void ControllerChat::receiveMessage(ModelMessage& message, const bool edited) const
{
    QString messageContent;
    ModelRoom& room = _model->getRoom(message.getIdRoom());

    if (room.isPrivate())
    {
        CypherText cypher(message.getContent().size());
        memcpy(cypher.data(), message.getContent().data(), message.getContent().size());
        messageContent = QString::fromStdString(_cryptor->decryptWithAES(cypher, room.getSecretKey()));
        message.setContent(messageContent.toUtf8());
    }

    if (!edited)
        _model->getRoom(room.getIdRoom()).addMessage(message);
    else
        _model->modifyMessage(room.getIdRoom(), message.getIdMessage(), message.getContent(), message.getEditionDate());

    _view->loadRoomMessage(message, edited);
}

void ControllerChat::userStatusChanged(const quint32 userId, const bool isConnected) const
{
    _view->userStatusChanged(userId, isConnected);
}

void ControllerChat::newMembershipRequest(const quint32 roomId, const ModelUser& user,
                                          const QByteArray& publicKey) const
{
    _model->addMembershipRequest(roomId, user, publicKey);
    // Add a new request.
    _view->updateRequests(1);
    _viewRequests->refresh(_model->getRequests());
}

void ControllerChat::openRoomModule(const bool editRoom) const
{
    if (editRoom)
        _controllerRoom->showRoom(_view->getSelectedRoomId());
    else
        _controllerRoom->showRoom();
}

void ControllerChat::loadRoomMessages(const quint32 idRoom) const
{
    ModelRoom& room = _model->getRoom(idRoom);

    _view->updateButtons(room.getAdmins().contains(_currentUser->getIdUser()));
    _view->loadRoomMessages(_model->getRoom(idRoom).getMessages());
}

void ControllerChat::loadUserRooms() const
{
    QList<quint32> userRooms = _model->getUserRooms(_currentUser->getIdUser());
    qStableSort(userRooms);

    for (quint32 roomId : userRooms)
    {
        ModelRoom& room = _model->getRoom(roomId);
        _view->addRoom(roomId, room.getName(), room.getPicture(), room.isPrivate());

        for (quint32 userId : room.getUsers())
        {
            ModelUser& user = _model->getUser(userId);
            _view->addUserToRoom(roomId, userId, user.getUserName(), user.getImage(), user.isConnected());
        }
    }

    _view->selectFirstRoom();
}

void ControllerChat::sendMessage() const
{
    QByteArray messageContent;
    ModelRoom& room = _model->getRoom(_view->getSelectedRoomId());

    if (room.isPrivate())
    {
        CypherText cypher(_cryptor->encryptWithAES(_view->getMessageText().toStdString(), room.getSecretKey()));
        messageContent = QByteArray((char*)cypher.data(), cypher.size());
    }
    else
    {
        messageContent = _view->getMessageText().toUtf8();
    }

    ModelMessage message(0, room.getIdRoom(), _currentUser->getIdUser(), QDateTime::currentDateTime(), QDateTime::currentDateTime(), messageContent);

    _co->sendMessage(message, false);
}

void ControllerChat::editMessage(const QTreeWidgetItem* item) const
{
    QByteArray messageContent;
    ModelRoom& room = _model->getRoom(_view->getSelectedRoomId());

    if (room.isPrivate())
    {
        CypherText cypher(_cryptor->encryptWithAES(item->text(1).toStdString(), room.getSecretKey()));
        messageContent = QByteArray((char*)cypher.data(), cypher.size());
    }
    else
    {
        //messageContent = QByteArray::fromStdString(item->text(1).toStdString());
        messageContent = item->text(1).toUtf8();
    }

    ModelMessage message(item->data(1, Qt::UserRole).toInt(), room.getIdRoom(), _currentUser->getIdUser(), QDateTime::currentDateTime(), QDateTime::currentDateTime(), messageContent);

    _co->sendMessage(message, true);
}

void ControllerChat::askServerToDeleteMessage(const quint32 roomId, const quint32 messageId) const
{
    _co->deleteMessage(roomId, messageId);
}

void ControllerChat::deleteMessageInModel(const quint32 roomId, const quint32 messageId) const
{
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
    _model->deleteRoom(roomId);
    _view->deleteRoom(roomId);
}

void ControllerChat::askServerToLeaveRoom(const quint32 roomId) const
{
    _co->leave(_currentUser->getIdUser(), roomId);
}

void ControllerChat::leaveRoomInModel(const quint32 userId, const quint32 roomId) const
{
    _model->removeUser(userId, roomId);

    if (userId == _currentUser->getIdUser())
        _view->deleteRoom(roomId);
    else
        _view->deleteUserFromRoom(userId, roomId);
}

void ControllerChat::showViewEdition()
{
    //Récupération de l'utilisateur
    _viewEdition->setCurrentUser(_currentUser);

    // Open the inscription window
    _viewEdition->show();
    _viewEdition->setEnabled(true);
}

void ControllerChat::openRoomMembership()
{
    _controllerRoom->showJoin();
}

void ControllerChat::showMembershipRequestsView()
{
    _viewRequests->refresh(_model->getRequests());
    _viewRequests->show();
}

void ControllerChat::processRequest(const bool accepted)
{
    ModelRequest request = _model->getRequest(_viewRequests->getSelectedRequestId());
    QByteArray aesKey;

    // If the user has been accepted in the room, we must send this room's secret key to the serveur.
    // To avoiding security issues, we encrypt this key with the requester-user public key.
    if (accepted)
    {
        AESKey roomSecretKey = request.getRoom().getSecretKey();
        
        qDebug() << "Acceptation de la demande d'adhésion";
        QByteArray tmp ((char*)roomSecretKey.key.data(), roomSecretKey.key.size());
        qDebug() << "Clé de la salle : " << tmp.toBase64();

        // Prepare a RSA key in which we store the requester-user public key.
        // In other words, we convert the requester-user public key from QByteArray to RSAPair.
        RSAPair rsaKey;
        rsaKey.publicKey.resize(request.getPublicKey().size());
        memcpy(rsaKey.publicKey.data(), request.getPublicKey().data(), rsaKey.publicKey.size());
        QByteArray tmp2 ((char*)request.getPublicKey().data(), request.getPublicKey().size());
        qDebug() << "Clé publique : " << tmp2.toBase64();
        // Encrypt the room's secret key with the requester-user public key (in RSAPair format).
        _cryptor->encryptWithRSA(roomSecretKey, rsaKey);
        qDebug() << "Clé est chiffrée";

        // Put the encrypted room's secret key in a QByteArray, for sending it to the server.
        QDataStream stream(&aesKey, QIODevice::WriteOnly);
        stream << QByteArray((char*)roomSecretKey.key.data(), roomSecretKey.key.size()) << QByteArray((char*)roomSecretKey.initializationVector.data(), roomSecretKey.initializationVector.size());
        QByteArray tmp3 ((char*)roomSecretKey.key.data(), roomSecretKey.key.size());
        qDebug() << "Clé de la salle : " << tmp3.toBase64();
        qDebug() << "Clé chiffrée : " << ;
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

ViewInscription* ControllerChat::getViewEdition()
{
    return _viewEdition;
}
