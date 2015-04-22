#ifndef CONTROLLER_USER_H
#define CONTROLLER_USER_H

#include "controllerDB.h"

class ControllerUser
{
    public:
    ControllerUser(ControllerDB& db);
    
    private:
    ControllerDB& _db;
};

#endif
