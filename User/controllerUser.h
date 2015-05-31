/*
     * Created by Mélanie Huck
     * Edited by Bastien Rouiller
     *
     * Handle user's interactions with the connection's windows and
     * interact with controllerOutput to send request to the server.
     *
*/

#ifndef CONTROLLERUSER_H
#define CONTROLLERUSER_H

#include <QString>
#include "viewUser.h"
#include "../ModeleChator/modelChator.h"
#include "../Serveur/controllerInput/clientControllerInput.h"
#include "../Serveur/Interpretor/interpretor.h"
#include "../Serveur/ServerConnector/connector.h"
#include "../Serveur/controllerOutput/controllerOutput.h"
#include "../Chat/controllerChat.h"
#include "../Cryptor/cryptor.h"

class ClientControllerInput;
class ControllerChat;


class ControllerUser : public QObject
{

    Q_OBJECT

private:

    ViewUser* _view;
    ModelChator* _model;

    ClientControllerInput* _cci;
    Interpretor* _i;
    ClientConnector* _cc;
    ControllerOutput* _co;
    ControllerChat* _controllerChat;

    Cryptor* _cryptor;

    ModelUser* _currentUser;

    // true -> connection is made from the login's window.
    // false -> connection is made from inscription's
    bool _fromBtnConnection;

    bool _connected;

public:
    ControllerUser(ModelChator* model, ModelUser* currentUser, ClientControllerInput* cci, Interpretor* i,
                   ClientConnector* cc, ControllerOutput* co, ControllerChat* controllerChat, Cryptor* cryptor);
    ~ControllerUser();

    //----------------------------------------------------------------------------------
    // Goal      : Open the connection's windows. This is the first window that the user
    //             see when launching the programm.
    // Param     : /
    //----------------------------------------------------------------------------------
    void showView() const;

    //----------------------------------------------------------------------------------
    // Goal      : Receives encrypted informations from the server such as keySalt and
    //             rsaKeys. Once the user has received it, he can use his password to
    //             decipher it and store it in the local app. It will allow him to
    //             decipher encrypted messages addressed to him.
    // Param     : user - ModelUser of the current user
    //             keySalt - salt used for key's encryption
    //             rsaKeys - RSA Keys
    //----------------------------------------------------------------------------------
    void infoUser(ModelUser& user, const Salt& keySalt, RSAPair& rsaKeys);


public slots:
    //----------------------------------------------------------------------------------
    // Goal      : Make a TCP connection with the server through the client connector.
    //             It is ClientConnector's responsability's to open a socket with the
    //             server. Connection is also made when user is opening the
    //             inscription's window so he won't create an account if the server is
    //             unreachable.
    //             If the user is already connected through a socket, we skip to next
    //             step.
    // Param     : _fromBtnConnection - true -> connection is made from the login's
    //                                          window.
    //                                - false -> connection is made from inscription's
    //----------------------------------------------------------------------------------
    void connectToServer(bool _fromBtnConnection);

    //----------------------------------------------------------------------------------
    // Goal      : This function is called during connectToServer.
    //             If the user want to log in, ask the server for salt. Look for the
    //             receiveSalt method for further information.
    //             If the user want to create a new account, there is no check needed.
    //             Inscription's window will open.
    // Param     : /
    //----------------------------------------------------------------------------------
    void auth();

    //----------------------------------------------------------------------------------
    // Goal      : Retrieve the user's information from the view. And generate a certain
    //             numbers of security related elements by using a Cryptor object :
    //             passwordSalt - Used to generate the hash, generated
    //             hashPassword - Made from passwordSalt and user password
    //             keyPair - RSA Key pair
    //             keySalt - Salt use to encrypt RSA Key pair.
    //             Then these elements will be sent to the server alongside user's
    //             information.
    //             For further information on the security aspect, refer to Cryptor
    //             source and developper's documentation.
    // Param     : /
    //----------------------------------------------------------------------------------
    void inscriptionToServer() const;

    //----------------------------------------------------------------------------------
    // Goal      : Use to edit any information about current user information except
    //             username. Send new information to server and update local current
    //             user.
    // Param     : /
    //----------------------------------------------------------------------------------
    void editUser() const;


    //----------------------------------------------------------------------------------
    // Goal      : Receiving salt used to hash user password. This method is
    //             automatically called when we server answer to the requestSalt
    //             request.
    //             Finally we can log in to server and acces the chat if the username
    //             and the password are correct.
    // Param     : /
    //----------------------------------------------------------------------------------
    void receiveSalt(const Salt& salt) const;

    //----------------------------------------------------------------------------------
    // Goal      : This method is used when a user cancel the inscription. Enable
    //             connection view.
    // Param     : /
    //----------------------------------------------------------------------------------
    void cancelInscription();

    //----------------------------------------------------------------------------------
    // Goal      : Display an user anthentication's error.
    // Param     : /
    //----------------------------------------------------------------------------------
    void authError();

};

#endif // CONTROLLERUSER_H
