#include "controllerChat.h"

ControllerChat::ControllerChat()
{
    view = new ViewChat();
}

ControllerChat::~ControllerChat()
{
    delete view;
}

void ControllerChat::showView()
{
    view->show();
}

void ControllerChat::loadRooms(const quint32 idUser)
{
    QList<ModelRoom*> userRooms = model->getUserRooms(idUser);

    for (ModelRoom* r : userRooms)
        view->addRoom();
}

void ControllerChat::addMessage(const quint32 idUser, const quint32 idRoom, const QString message)
{
    printf("SENT");
}
