// Controller of the room module.

#ifndef CHAT_ROOM_CONTROLLER
#define CHAT_ROOM_CONTROLLER

#include <QString>

#include "viewRoom.h"
#include "../../ModeleChator/modelChator.h"

class ControllerRoom : public QObject
{
    Q_OBJECT
private:

    // Pointers on the view(s) and the model.
    ViewRoom* viewRoom;
    ModelChator* model;
    
    // Load members of a room into the model of the list view.
    void loadMembers(const ModelRoom& room);
    // Add a member into the model of the list view.
    void addMember(const QString name);
    
public:

    ControllerRoom();
    ~ControllerRoom();
    
    // Open the room's creation window.
    void showRoom();
    
    // Open the room's edition window, loading informations from the passed room.
    void showRoom(const quint32 idRoom);
    
public slots :

    // Add the current name entered into the members list. 
    void addMember();
    
    // Remove the selected member, or all of them.
    void removeMember();
    void removeMembers();

};


#endif //CHAT_ROOM_CONTROLLER
