#include "controllerRoom.h"

ControllerRoom::ControllerRoom()
{
    view = new ViewRoom();
}

ControllerRoom::~ControllerRoom()
{
    delete view;
}

void ControllerRoom::show()
{
    view->show();
}

void ControllerRoom::loadMembers(const int idRoom)
{
    ModelRoom room = model->getRoom(idRoom);
    
    //for(ModelUser user : room.
}

/*
void ControllerRoom::addMember(const QString name)
{
    sim_members->appendRow(new QStandardItem("Franz"));
    sim_members->appendRow(new QStandardItem("Garry"));
    sim_members->appendRow(new QStandardItem("Georges"));
}

void ViewRoom::addMember()
{
    if (!ldt_membre->text().isEmpty())
    {
        QStandardItem* item = new QStandardItem(ldt_membre->text());
        item->setEditable(false);
        sim_members->appendRow(item);
    }
}*/
