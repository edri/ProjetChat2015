#ifndef CHAT_ROOM_CONTROLLER
#define CHAT_ROOM_CONTROLLER

#include <QString>

#include "viewRoom.h"
#include "../../ModeleChator/modelChator.h"

class ControllerRoom : public QObject
{
    Q_OBJECT
private:
    ViewRoom* view;
    ModelChator* model;
    
public:
    ControllerRoom();
    ~ControllerRoom();
    
    void show();
    
public slots :
    void loadMembers(const int idRoom);
    //void addMember(const QString name);

};


#endif //CHAT_ROOM_CONTROLLER
