#ifndef CONTROLLER_ROOM_H
#define CONTROLLER_ROOM_H

#include "controllerDB.h"

class ControllerRoom
{
    public:
    ControllerRoom(ControllerDB& db);
    
    private:
    ControllerDB& _db;
};

#endif
