/*
 * File : controllerRoom.h
 * Project : ProjetChat2015
 * Author(s) : Jan Purro
 * Last Modified : 25.04.2015 14:25 by Jan Purro
 * Description : Controller for the room module.
 * 
 * The room module manage chat room's creation and edition(management) as well 
 * as user joining a room.
 * Two views are available in this module : one for creation/edition and one for
 * joining called respectively ViewRoom and ViewJoin.
 * The model is the application model (ModelChator).
 * 
 * The controller will open views when needed, retriew and insert data from/in 
 * the model and take care of the communications with the server for this
 * module.
 */

#ifndef CHAT_CONTROLLER_ROOM
#define CHAT_CONTROLLER_ROOM

#include <QString>

#include "viewRoom.h"
// These paths will be probably changed at some time.
#include "../../ModeleChator/modelChator.h"
#include "../Join/viewJoin.h"
#include "../../Serveur/controllerOutput/controllerOutput.h"

class ControllerRoom : public QObject
{
    Q_OBJECT
private:

    // Used for test
    int bidon;
    
    // Pointers on the view(s) and the model.
    ModelChator* model;
    ViewRoom* viewRoom;
    ViewJoin* viewJoin;
    ControllerOutput* controllerOutput;
    
    
    // Pinter on the current user.
    ModelUser* currentUser;
    
    // Connect the viewRoom signals with the controller's slots. Should only
    // be called when the viewRoom is a valid pointer(seg fault otherwise).
    void connectViewRoom();
    // Connect the viewJoin signals with the controller's slots. Should only
    // be called when the viewJoin is a valid pointer(seg fault otherwise).
    void connectViewJoin();
    
    // Check that the file found at path is a valid image.
    bool isValidImage(const QString& path);
    
public:

    // Constructor.
    ControllerRoom(ModelChator* model, ModelUser* user, ControllerOutput* controllerOutput);
    
    // Open the room's creation window.
    void showRoom();
    // Open the room's edition window, loading informations from the passed room.
    void showRoom(const quint32 idRoom);
    
    // Should be called when a user info is sent from the server to the controller
    void addUser(const quint32 userId, const QString& userName);
    // Should be called when a userDoesNotExist error is sent .
    void userDoesNotExist();
    // Should be called when a Room message is received by the client.
    void roomConfirmation(const ModelRoom& room, bool edited);
    
    
public slots :

    // Creation of a room. Get the data from the viewRoom and send them
    // to the server
    void createRoom();
    
    // Edition of the loaded room.Get the data from the viewRoom and send them
    // to the server
    void editRoom();
    // Get the name entered by the user from the viewRoom and ask the server for
    // the corresponding userId (if the user exists).
    void addUser();
    
    // Send the server a request for the user to join a specific room.
    void joinRoom();
    
    // Close and destroy viewRoom. Should only be called if a window is open.
    void cancelRoom();
    // Close and destroy viewJoin. Should only be called if a window is open.
    void cancelJoin();

};


#endif //CHAT_CONTROLLER_ROOM
