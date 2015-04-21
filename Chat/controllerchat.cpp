#include "controllerChat.h"

ControllerChat::ControllerChat(ModelChator* model, ModelUser* currentUser)
{
    _view = new ViewChat();
    _model = model;
    _currentUser = currentUser;

    connect(_view, SIGNAL(requestLoadRoomMessages(const quint32)), this, SLOT(loadRoomMessages(const quint32)));
}

ControllerChat::~ControllerChat()
{
    delete _view;
    delete _model;
}

void ControllerChat::showView() const
{
    _view->show();

    _view->setConnectedAsText(_currentUser->getUserName());
    loadRooms(_currentUser->getIdUser());
}

void ControllerChat::loadRoomMessages(const quint32 idRoom) const
{
    _view->loadRoomMessage(1, "edri", "4", QDateTime::fromString("04.04.2015 20:17", "dd.MM.yyyy HH:mm"));
    _view->loadRoomMessage(1, "jan", "3", QDateTime::fromString("04.04.2015 19:17", "dd.MM.yyyy HH:mm"));
    _view->loadRoomMessage(1, "ben", "1", QDateTime::fromString("01.04.2015 20:17", "dd.MM.yyyy HH:mm"));
    _view->loadRoomMessage(1, "b2b", "6", QDateTime::fromString("05.04.2015 20:17", "dd.MM.yyyy HH:mm"));
    _view->loadRoomMessage(1, "mel", "2", QDateTime::fromString("01.04.2015 20:17", "dd.MM.yyyy HH:mm"));
    _view->loadRoomMessage(1, "mel", "5", QDateTime::fromString("04.04.2015 21:17", "dd.MM.yyyy HH:mm"));
    _view->loadRoomMessage(1, "edri", "7", QDateTime::currentDateTime());
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
