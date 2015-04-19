#include "controllerChat.h"

ControllerChat::ControllerChat(ModelChator* model, ModelUser* currentUser)
{
    _view = new ViewChat();
    _model = model;
    _currentUser = currentUser;
}

ControllerChat::~ControllerChat()
{
    delete _view;
    delete _model;
}

void ControllerChat::showView()
{
    _view->show();

    _view->setConnectedAsText(_currentUser->getUserName());
    loadRooms(_currentUser->getIdUser());
}

void ControllerChat::loadRooms(const quint32 idUser)
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
