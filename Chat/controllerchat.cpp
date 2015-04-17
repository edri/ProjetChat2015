#include "controllerChat.h"

ControllerChat::ControllerChat(ModelChator* model)
{
    view = new ViewChat();
    this->model = model;
}

ControllerChat::~ControllerChat()
{
    delete view;
    delete model;
}

void ControllerChat::showView()
{
    view->show();
}

void ControllerChat::loadRooms(const quint32 idUser)
{
    QMap<quint32, ModelRoom*> userRooms = model->getUserRooms(idUser);

    for (ModelRoom* r : userRooms)
        view->addRoom(r->getName(), r->getIdRoom());
}

void ControllerChat::addMessage(const quint32 idUser, const quint32 idRoom, const QString message)
{
    printf("SENT");
}
