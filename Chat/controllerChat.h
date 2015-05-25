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

    void showView() const;
    void loadUser(ModelUser& user) const;
    void loadRoom(ModelRoom& room) const;
    void receiveMessage(ModelMessage& message, const bool edited) const;
    void userStatusChanged(const quint32 userId, const bool isConnected) const;
    void newNotification(const NotificationType notifType) const;

public slots :
    void loadUserRooms() const;
    void openRoomModule() const;
    void loadRoomMessages(const quint32 idRoom) const;
    void sendMessage() const;
    void editMessage(const QTreeWidgetItem* item) const;
    void askServerToDeleteMessage(const quint32 roomId, const quint32 messageId) const;
    void deleteMessageInModel(const quint32 roomId, const quint32 messageId) const;
    void askServerToDeleteRoom(const quint32 roomId) const;
    void deleteRoomInModel(const quint32 roomId) const;
    void askServerToLeaveRoom(const quint32 roomId) const;
    void leaveRoomInModel(const quint32 roomId) const;

};

#endif // CONTROLLERCHAT
