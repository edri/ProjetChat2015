#include "roomController.h"

RoomController::RoomController()
{
    room = new Room(this);
}

RoomController::~RoomController()
{
    delete room;
}

void RoomController::show()
{
    room->show();
}

void RoomController::loadMembers(const int idRoom)
{
    ModelRoom room = model->getRoom(idRoom);
    
    //for(ModelUser user : room.
}

/*
void RoomController::addMember(const QString name)
{
    sim_members->appendRow(new QStandardItem("Franz"));
    sim_members->appendRow(new QStandardItem("Garry"));
    sim_members->appendRow(new QStandardItem("Georges"));
}

void Room::addMember()
{
    if (!ldt_membre->text().isEmpty())
    {
        QStandardItem* item = new QStandardItem(ldt_membre->text());
        item->setEditable(false);
        sim_members->appendRow(item);
    }
}*/
