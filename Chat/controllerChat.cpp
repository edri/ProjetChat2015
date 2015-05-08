#include "controllerChat.h"

ControllerChat::ControllerChat(ModelChator* model, ModelUser* currentUser, ClientControllerInput* cci,
                               Interpretor* i, ClientConnector* cc, ControllerOutput* co, ControllerRoom* controllerRoom)
{
    _view = new ViewChat();

    _model = model;
    _currentUser = currentUser;
    _cci = cci;
    _i = i;
    _cc = cc;
    _co = co;
    _controllerRoom = controllerRoom;

    connect(_view, SIGNAL(requestOpenRoomModule()), this, SLOT(openRoomModule()));
    connect(_view, SIGNAL(requestLoadRoomMessages(const quint32)), this, SLOT(loadRoomMessages(const quint32)));
    connect(_view, SIGNAL(requestSendMessage()), this, SLOT(sendMessage()));
    connect(_view, SIGNAL(requestEditMessage(const QTreeWidgetItem*)), this, SLOT(editMessage(const QTreeWidgetItem*)));
}

ControllerChat::~ControllerChat()
{
    delete _view;
    delete _model;
}

void ControllerChat::showView() const
{
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

void ControllerChat::receiveMessage(ModelMessage& message) const
{
    _model->getRoom(message.getIdRoom()).addMessage(message);
    _view->loadRoomMessage(message.getIdRoom(), message.getIdMessage(), _model->getUser(message.getIdUser()).getUserName(),
                           message.getContent(), message.getDate(), (message.getIdUser() == _currentUser->getIdUser()));
}

void ControllerChat::openRoomModule() const
{
    _controllerRoom->showRoom();
}

void ControllerChat::loadRoomMessages(const quint32 idRoom) const
{
    ModelRoom& room = _model->getRoom(idRoom);

    for (ModelMessage& message : room.getMessages())
    {
        _view->loadRoomMessage(idRoom, message.getIdMessage(), _model->getUser(message.getIdUser()).getUserName(),
                               message.getContent(), message.getDate(), (message.getIdUser() == _currentUser->getIdUser()));
    }
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
    ModelMessage message(0, _view->getSelectedRoomId(), _currentUser->getIdUser(), QDateTime::currentDateTime(), _view->getMessageText());

    _co->sendMessage(message, false);
}

void ControllerChat::editMessage(const QTreeWidgetItem* item) const
{
    ModelMessage message(item->data(0, Qt::UserRole).toInt(), _view->getSelectedRoomId(), _currentUser->getIdUser(), QDateTime::currentDateTime(), item->text(1));

    _co->sendMessage(message, true);
}
