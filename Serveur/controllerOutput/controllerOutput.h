#ifndef CONTROLLER_OUTPUT_H
#define CONTROLLER_OUTPUT_H

#include <QtCore/QObject>
#include "../Interpretor/interpretor.h"

class ControllerOutput
{
    public:
    ControllerOutput(Connector& connector, Interpretor& interpretor);
    
    void sendMessage(const ModelMessage& message, const bool edited);

    /* Called when an user want to delete its message.
     * - messageId : the message's id we want to delete.
     * Created by Miguel Santamaria on 16.05.2015 21:30
     */
    void deleteMessage(const quint32 roomId, const quint32 messageId);

    void login(const QString& pseudo, const Hash& hashedPwd);
    void createAccount(const ModelUser& user, const Hash& password);
    void editAccount(const ModelUser& user);
    void sendInfoUser(const ModelUser& user);
    void join(const quint32 idUser, const quint32 idRoom);
    void leave(const quint32 idUser, const quint32 idRoom);
    void disconnect(const quint32 idUser);
    
    /* Called when creating or editing a room.
     * - room : the created or edited room.
     * - edited : false if the room is created, true if the room is an already existing modfied room.
     * 
     * Oui je sais le nom est peut être pas le plus génial qui soit. Mais bon createRoom ou editRoom c'est pas cool non plus, alors tant que c'est le seul truc qui concerne directement les salles autant le garder.
     * Created by Jan Purro on 26.04.2015 14:46
     */
    void room(const ModelRoom& room, bool edited = false);
    
    /* Called when asking the server for the ID corresponding to a user name, provided the user exists.
     *  - userName : the inquired user name.
     *  - exists : only used by the server when responding 
     *  - userId : only used by the server when responding 
     */
    void userId(const QString& userName, bool exists = false, quint32 userId = 0);

    /* Called when a room administrator want to delete it.
     * - roomId : the room's id we want to delete.
     * Created by Miguel Santamaria on 13.05.2015 08:52
     */
    void deleteRoom(const quint32 roomId);

    /* Called when an user want to leave a room.
     * - roomId : the room's id the user want to leave.
     * Created by Miguel Santamaria on 14.05.2015 18:31
     */
    void leaveRoom(const quint32 roomId);

    /* Called when a user need a salt in order to hash his password
     * when he is trying to connect to the server
     * - pseudo : userName
     * Create by Bastien Rouiller on 25.05.2015
     */
    //void askForSalt(const QString& pseudo);


    private:
    Connector& _connector;
    Interpretor& _interpretor;
};

#endif
