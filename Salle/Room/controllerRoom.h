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
#include "../../Cryptor/cryptor.h"

class ControllerRoom : public QObject
{
    Q_OBJECT
private:
    
    // Pointers on the view(s) and the model.
    ModelChator* _model;
    ViewRoom* _viewRoom;
    ViewJoin* _viewJoin;
    ControllerOutput* _controllerOutput;
    Cryptor* _cryptor;
    quint32 _currentRoomId;
    
    // Pointer on the current user.
    ModelUser* _currentUser;
    
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
    ControllerRoom(ModelChator* model, ModelUser* user, ControllerOutput* controllerOutput, Cryptor* cryptor);
    
    // Open the room's creation window.
    void showRoom();
    // Open the room's edition window, loading informations from the passed room.
    void showRoom(const quint32 idRoom);
    // Should be called when a user's id is received by the client.
    void userId(bool exists, quint32 userId);
    // Open the romm joining window.
    void showJoin();
    // Shoulde be called when the lists of visible rooms are received by the 
    // Client
    void listRooms(const QList<QPair<quint32, QString>>& publicRooms,
                   const QList<QPair<quint32, QString>>& privateRooms);
    // Close all opened windows.
    void closeWindows();
    
    
public slots :

    // Creation of a room. Get the data from the viewRoom and send them
    // to the server
    void createRoom();
    void createRoom(QList<QPair<quint32, QByteArray>>& idsAndKeys);
    
    // Edition of the loaded room.Get the data from the viewRoom and send them
    // to the server
    void editRoom();
    // Get the name entered by the user from the viewRoom and ask the server for
    // the corresponding userId (if the user exists).
    void addUser();
    
    // Send the server a request for the user to join a specific room.
    void joinRoom(quint32 roomId);
    
    // Close and destroy viewRoom. Should only be called if a window is open.
    void cancelRoom();
    // Close and destroy viewJoin. Should only be called if a window is open.
    void cancelJoin();

};


#endif //CHAT_CONTROLLER_ROOM
