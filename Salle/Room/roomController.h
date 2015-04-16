#ifndef CHAT_ROOM_CONTROLLER
#define CHAT_ROOM_CONTROLLER

#include <QString>

#include "room.h"
#include "../../ModeleChator/modeleChator.h"

class RoomController : public QObject
{
    Q_OBJECT
private:
    Room* room;
    ModelChator* model;
    
public:
    RoomController();
    ~RoomController();
    
    void show();
    
public slots :
    void loadMembers(const int idRoom);
    //void addMember(const QString name);

};


#endif //CHAT_ROOM_CONTROLLER
