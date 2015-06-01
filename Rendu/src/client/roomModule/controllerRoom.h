/*
 * File : controllerRoom.h
 * Project : ProjetChat2015
 * 
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
#include "../../common/models/modelChator.h"
#include "viewJoin.h"
#include "../../Serveur/controllerOutput/controllerOutput.h"
#include "../../common/cryptor/cryptor."

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
    // idRoom : the room whose informations will be loaded.
    void showRoom(const quint32 idRoom);
    // Should be called when a user's id is received by the client.
    void userId(bool exists, quint32 userId);
    // Open the window for the room adhesion form.
    void showJoin();
    // Shoulde be called when the lists of visible rooms are received by the 
    // client
    void listRooms(const QList<QPair<quint32, QString>>& publicRooms,
                   const QList<QPair<quint32, QString>>& privateRooms);
    // Close all opened windows.
    void closeWindows();
    
    
public slots :

    // Creation of a room. Get the data from the viewRoom and send them
    // to the server.
    // idsAndKeys : the IDs and public keys of the users. Needed when creating
    // a private key to encrypt the room's key.
    void createRoom();
    void createRoom(QList<QPair<quint32, QByteArray>>& idsAndKeys);
    
    // Edition of the loaded room.Get the data from the viewRoom and send them
    // to the server
    void editRoom();
    // Get the name entered by the user from the viewRoom and ask the server for
    // the corresponding userId (if the user exists).
    void addUser();
    
    // Send the server a request for the user to join a specific room.
    // roomId : the id of the room to be joined.
    // isPrivate : if the room is a private room.
    void joinRoom(quint32 roomId, bool isPrivate);
    
    // Close and destroy viewRoom. Should only be called if a window is open.
    void cancelRoom();
    // Close and destroy viewJoin. Should only be called if a window is open.
    void cancelJoin();

    // Request to reactivate the chat windows.
    void askReactivateChatWindows();

signals:
    // Reactivate the chat windows.
    void reactivateChatWindows();

};


#endif //CHAT_CONTROLLER_ROOM
