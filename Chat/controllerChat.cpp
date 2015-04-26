#include "controllerChat.h"

ControllerChat::ControllerChat(ModelChator* model, ModelUser* currentUser, ClientControllerInput* cci,
                               Interpretor* i, ClientConnector* cc, ControllerOutput* co)
{
    _view = new ViewChat();

    _model = model;
    _currentUser = currentUser;
    _cci = cci;
    _i = i;
    _cc = cc;
    _co = co;

    connect(_view, SIGNAL(requestLoadRoomMessages(const quint32)), this, SLOT(loadRoomMessages(const quint32)));
    connect(_view, SIGNAL(requestSendMessage()), this, SLOT(sendMessage()));

    // TEST ; A SUPPRIMER PAR LA SUITE.
    connect(_cc, SIGNAL(connectionSuccessful()), this, SLOT(auth()));
}

ControllerChat::~ControllerChat()
{
    delete _view;
    delete _model;
}

void ControllerChat::showView() const
{
    _view->setConnectedAsText(_currentUser->getUserName());
    loadRooms(_currentUser->getIdUser());

    _view->show();
}

void ControllerChat::loadRoomMessages(const quint32 idRoom) const
{
    ModelRoom& room = _model->getRoom(idRoom);

    for (ModelMessage& message : room.getMessages())
    {
        _view->loadRoomMessage(message.getIdMessage(), _model->getUser(message.getIdUser()).getUserName(), message.getContent(), message.getDate());
    }
}

void ControllerChat::loadRooms(const quint32 idUser) const
{
    QList<quint32> userRooms = _model->getUserRooms(idUser);
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

// TEST ; A SUPPRIMER PAR LA SUITE.
void ControllerChat::auth()
{
    ModelMessage message(0, 0, 0, QDateTime::currentDateTime(), "");
    _co->login("licorne", "java");
}

void ControllerChat::sendMessage() const
{
    ModelMessage message(0, _view->getSelectedRoomId(), _currentUser->getIdUser(), QDateTime::currentDateTime(), _view->getMessageText());

    _co->sendMessage(message);
}
