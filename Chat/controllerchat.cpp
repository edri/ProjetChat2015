#include "controllerChat.h"

ControllerChat::ControllerChat(ModelChator* model, ModelUser* currentUser)
{
    _view = new ViewChat();
    _model = model;
    _currentUser = currentUser;

    cci = new ClientControllerInput();
    i = new Interpretor(*cci);
    cc = new ClientConnector();

    connect(cc, SIGNAL(connectionSuccessful()), this, SLOT(auth()));

    co = new ControllerOutput(*cc, *i);
    cc->connectToServer("localhost:1234");

    connect(_view, SIGNAL(requestLoadRoomMessages(const quint32)), this, SLOT(loadRoomMessages(const quint32)));
    connect(_view, SIGNAL(requestSendMessage()), this, SLOT(sendMessage()));
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
    ModelRoom* room = _model->getRoom(idRoom);

    for (ModelMessage* message : room->getMessages())
    {
        _view->loadRoomMessage(message->getIdMessage(), _model->getUser(message->getIdUser())->getUserName(), message->getContent(), message->getDate());
    }
}

void ControllerChat::loadRooms(const quint32 idUser) const
{
    QMap<quint32, ModelRoom*> userRooms = _model->getUserRooms(idUser);

    for (ModelRoom* room : userRooms)
    {
        _view->addRoom(room->getIdRoom(), room->getName(), room->getPicture());

        for (ModelUser* user : room->getUsers())
        {
            _view->addUserToRoom(room->getIdRoom(), user->getIdUser(), user->getUserName(), user->getImage(), user->isConnected());
        }
    }

    _view->selectFirstRoom();
}

void ControllerChat::auth()
{
    ModelMessage message(0, 0, 0, QDateTime::currentDateTime(), "");

    co->login("Jean-paul", "fromage66");
}

void ControllerChat::sendMessage() const
{
    ModelMessage message(0, _view->getSelectedRoomId(), _currentUser->getIdUser(), QDateTime::currentDateTime(), _view->getMessageText());

    co->login("Jean-paul", "fromage66");
    //co->sendMessage(message);
}
