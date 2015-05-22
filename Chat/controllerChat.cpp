#include "controllerChat.h"

ControllerChat::ControllerChat(ModelChator* model, ModelUser* currentUser, ClientControllerInput* cci,
                               Interpretor* i, ClientConnector* cc, ControllerOutput* co, ControllerRoom* controllerRoom,
                               Cryptor* cryptor)
{
    _model = model;
    _view = new ViewChat(_model);

    _currentUser = currentUser;
    _cci = cci;
    _i = i;
    _cc = cc;
    _co = co;
    _controllerRoom = controllerRoom;
    _cryptor = cryptor;

    connect(_view, SIGNAL(requestOpenRoomModule()), this, SLOT(openRoomModule()));
    connect(_view, SIGNAL(requestLoadRoomMessages(const quint32)), this, SLOT(loadRoomMessages(const quint32)));
    connect(_view, SIGNAL(requestSendMessage()), this, SLOT(sendMessage()));
    connect(_view, SIGNAL(requestEditMessage(const QTreeWidgetItem*)), this, SLOT(editMessage(const QTreeWidgetItem*)));
    connect(_view, SIGNAL(requestDeleteMessage(quint32, quint32)), this, SLOT(askServerToDeleteMessage(quint32, quint32)));
    connect(_view, SIGNAL(requestDeleteRoom(quint32)), this, SLOT(askServerToDeleteRoom(quint32)));
    connect(_view, SIGNAL(requestLeaveRoom(quint32)), this, SLOT(askServerToLeaveRoom(quint32)));
}

ControllerChat::~ControllerChat()
{
    delete _view;
    delete _model;
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
    _model->addRoom(room);
}

void ControllerChat::receiveMessage(ModelMessage& message, const bool edited) const
{
    if (!edited)
        _model->getRoom(message.getIdRoom()).addMessage(message);
    else
        _model->modifyMessage(message.getIdRoom(), message.getIdMessage(), message.getContent(), message.getEditionDate());

    _view->loadRoomMessage(message, edited);
}

void ControllerChat::userStatusChanged(const quint32 userId, const bool isConnected) const
{
    _view->userStatusChanged(userId, isConnected);
}

void ControllerChat::newNotification(const NotificationType notifType) const
{
    _view->newNotification(notifType);
}

void ControllerChat::openRoomModule() const
{
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
        _view->addRoom(roomId, room.getName(), room.getPicture());

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
    ModelMessage message(0, _view->getSelectedRoomId(), _currentUser->getIdUser(), QDateTime::currentDateTime(), QDateTime::currentDateTime(), _view->getMessageText());

    _co->sendMessage(message, false);
}

void ControllerChat::editMessage(const QTreeWidgetItem* item) const
{
    ModelMessage message(item->data(1, Qt::UserRole).toInt(), _view->getSelectedRoomId(), _currentUser->getIdUser(), QDateTime::currentDateTime(), QDateTime::currentDateTime(), item->text(1));

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
    _co->leaveRoom(roomId);
}

void ControllerChat::leaveRoomInModel(const quint32 roomId) const
{
    _model->removeUser(_currentUser->getIdUser(), roomId);
    _view->deleteRoom(roomId);
}
