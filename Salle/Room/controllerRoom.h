// Controller of the room module.

#ifndef CHAT_CONTROLLER_ROOM
#define CHAT_CONTROLLER_ROOM

#include <QString>

#include "viewRoom.h"
#include "../../ModeleChator/modelChator.h"
#include "../Join/viewJoin.h"

class ControllerRoom : public QObject
{
    Q_OBJECT
private:

    // Pointers on the view(s) and the model.
    ModelChator* model;
    ViewRoom* viewRoom;
    ViewJoin* viewJoin;
    
    // Pointer on the current user.
    ModelUser* user;
    
    // Connect signals from the views
    void connectViewRoom();
    void connectViewJoin();
    // Creation of a room following the fields entered.
    void createRoom();
    // Edition of the loaded room following the modified fields.
    void editRoom();
    
    
public:

    ControllerRoom(ModelChator* model, ModelUser* const user);
    
    // Open the room's creation window.
    void showRoom();
    // Open the room's edition window, loading informations from the passed room.
    void showRoom(const quint32 idRoom);
    
    // Should be called when a user info is sent from the server to the controller
    void addUser(const quint32 userId, const QString& userName);
    // Should be called when a userDoesNotExist error is sent .
    void userDoesNotExist();
    
    
public slots :

    // Add the current name entered into the members list. 
    void addUser();
    
    // Remove the selected member.
    void removeUser(const quint32 userId);
    
    // Call the creation respectively edition of a room according to the 
    // state of the room view.
    void actionRoom();
    
    void toggleAdmin();
    
    void joinRoom();
    
    void cancelRoom();
    void cancelJoin();

};


#endif //CHAT_CONTROLLER_ROOM
