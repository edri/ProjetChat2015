/*
     * Created by Miguel Santamaria
     *
     * Handle display of informations such as messages and rooms received
     * through controllerInput into viewChat.
     * Sent user requests such as room's suppressions or message's modifcation
     * to the controllerOutput.
*/

#ifndef CONTROLLERCHAT
#define CONTROLLERCHAT

#include <QString>

#include "viewChat.h"
#include "../ModeleChator/modelChator.h"
#include "../Serveur/controllerInput/clientControllerInput.h"
#include "../Serveur/Interpretor/interpretor.h"
#include "../Serveur/ServerConnector/connector.h"
#include "../Serveur/controllerOutput/controllerOutput.h"
#include "../Salle/Room/controllerRoom.h"
#include "../Cryptor/cryptor.h"

class ClientControllerInput;

class ControllerChat : public QObject
{
    Q_OBJECT
private:
    ViewChat* _view;
    ViewInscription* _viewEdition;
    ModelChator* _model;
    ModelUser* _currentUser;
    ClientControllerInput* _cci;
    Interpretor* _i;
    ClientConnector* _cc;
    ControllerOutput* _co;
    ControllerRoom* _controllerRoom;
    Cryptor* _cryptor;

public:
    ControllerChat(ModelChator* model, ModelUser* currentUser, ClientControllerInput* cci,
                   Interpretor* i, ClientConnector* cc, ControllerOutput* co, ControllerRoom* controllerRoom,
                   Cryptor* cryptor);
    ~ControllerChat();

    //----------------------------------------------------------------------------------
    // Goal      : Show the chat view.
    // Param     : /
    //----------------------------------------------------------------------------------
    void showView() const;

    //----------------------------------------------------------------------------------
    // Goal      : Add a new user in the local modelChator.
    // Param     : user - user that will be stored.
    //----------------------------------------------------------------------------------
    void loadUser(ModelUser& user) const;

    //----------------------------------------------------------------------------------
    // Goal      : Add a new user in the local modelChator.
    // Param     : room - room that will be stored.
    //----------------------------------------------------------------------------------
    void loadRoom(ModelRoom& room) const;

    //----------------------------------------------------------------------------------
    // Goal      : Add or modify a message in the corresponding room in the local
    //             modelChator.
    // Param     : message - message that will be stored or modified.
    //             edited - true -> modifiy an existing message
    //                      false-> adding a new message
    //----------------------------------------------------------------------------------
    void receiveMessage(ModelMessage& message, const bool edited) const;

    //----------------------------------------------------------------------------------
    // Goal      : Inform the view that a user's status has changed. For example a user
    //             the user went online.
    // Param     : userId - The affected user
    //             isConnected - The new user's status
    //----------------------------------------------------------------------------------
    void userStatusChanged(const quint32 userId, const bool isConnected) const;

    //----------------------------------------------------------------------------------
    // Goal      : Forward a notification to the view
    // Param     : notifType - type of notification, for example a new membership
    //----------------------------------------------------------------------------------
    void newNotification(const NotificationType notifType) const;

    //----------------------------------------------------------------------------------
    // Goal      : Return the viewEdition which is based on viewInscription
    // Param     : /
    // Return    : ViewInscription - viewEdition based on viewInscription
    //----------------------------------------------------------------------------------
    ViewInscription* getViewEdition();
	
    //----------------------------------------------------------------------------------
    // Goal      :
    // Param     :
    //----------------------------------------------------------------------------------
    void loadUserRooms() const;

public slots :
// Methods called when the controller received signal from the viewChat.
// The controller will then ask the controllerOutput to send request to the server
// or interact with other controllers.

    //----------------------------------------------------------------------------------
    // Goal      : Ask the controllerRoom to open a windows for room creation
    // Param     : /
    //----------------------------------------------------------------------------------
    void openRoomModule() const;

    //----------------------------------------------------------------------------------
    // Goal      : Load the messages from a room and send the information to the view
    // Param     : idRoom - The room we want to get the messages from
    //----------------------------------------------------------------------------------
    void loadRoomMessages(const quint32 idRoom) const;

    //----------------------------------------------------------------------------------
    // Goal      : Forward the message contained in the message field to the
    //             controllerOutput. This message will then be sent to the server.
    // Param     : /
    //----------------------------------------------------------------------------------
    void sendMessage() const;

    //----------------------------------------------------------------------------------
    // Goal      : Inform the controllerOutput that a message has to be modified.
    // Param     : item - An item containing the message in the QTree. QTree are used to
    //             show the messages in a structured way.
    //----------------------------------------------------------------------------------
    void editMessage(const QTreeWidgetItem* item) const;

    //----------------------------------------------------------------------------------
    // Goal      : Inform the controllerOutput that a user want to delete a message so
    //             and that the server should update his database.
    // Param     : roomId - Identify the room where the message should be deleted.
    //             messageId - Identify the message that has to be deleted.
    //----------------------------------------------------------------------------------
    void askServerToDeleteMessage(const quint32 roomId, const quint32 messageId) const;

    //----------------------------------------------------------------------------------
    // Goal      : Update the ModelChator in order to delete a message and also update
    //             the viewChat so the the message is not show anymore on the window.
    // Param     : roomId - Identify the room where the message should be deleted.
    //             messageId - Identify the message that has to be deleted
    //----------------------------------------------------------------------------------
    void deleteMessageInModel(const quint32 roomId, const quint32 messageId) const;

    //----------------------------------------------------------------------------------
    // Goal      : Inform the controllerOutput that a user want to delete a room so
    //             and that the server should update his database.
    // Param     : roomId - Identify the room that has to be deleted.
    //----------------------------------------------------------------------------------
    void askServerToDeleteRoom(const quint32 roomId) const;

    //----------------------------------------------------------------------------------
    // Goal      : Update the ModelChator in order to delete a message and also update
    //             the viewChat so the the message is not showed anymore on the window.
    // Param     : roomId - Identify the room where the message should be deleted.
    //             messageId - Identify the message that has to be deleted
    //----------------------------------------------------------------------------------
    void deleteRoomInModel(const quint32 roomId) const;

    //----------------------------------------------------------------------------------
    // Goal      : Inform the controllerOutput that a user want to delete a message so
    //             and that the server should update his database.
    // Param     : roomId - Identify the room that the user is leaving
    //----------------------------------------------------------------------------------
    void askServerToLeaveRoom(const quint32 roomId) const;

    //----------------------------------------------------------------------------------
    // Goal      : Update the ModelChator in order to be removed from a room and also
    //             update the viewChat so the the room is not showed anymore.
    // Param     : roomId - Identify the room where the message should be deleted.
    //             messageId - Identify the message that has to be deleted
    //----------------------------------------------------------------------------------
    void leaveRoomInModel(const quint32 roomId) const;

    //----------------------------------------------------------------------------------
    // Goal      : Open the edition's view.
    // Param     : /
    //----------------------------------------------------------------------------------
    void showViewEdition();

};

#endif // CONTROLLERCHAT
