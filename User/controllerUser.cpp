/*
     * Created by Mélanie Huck
     * Edited by Bastien Rouiller
     *
     * Implements controllerUser.h
     *
*/

#include "controllerUser.h"

ControllerUser::ControllerUser(ModelChator* model, ModelUser* currentUser, ClientControllerInput* cci, Interpretor* i, ClientConnector* cc,
                               ControllerOutput* co, ControllerChat* controllerChat, Cryptor* cryptor)
{
    _cci = cci;
    _i = i;
    _cc = cc;
    _co = co;
    _cryptor = cryptor;
    // Initialize a new view for the connection
    _view = new ViewUser();
    _model = model;
    _currentUser = currentUser;
    _controllerChat = controllerChat;
    _connected = false;

    // Bind the signals and the slots
    connect(_view, SIGNAL(requestGetIds(bool)), this, SLOT(connectToServer(bool)));
    connect(cc, SIGNAL(connectionSuccessful()), this, SLOT(auth()));
    connect(_view->getViewInscription(), SIGNAL(requestGetNewUser()), this, SLOT(inscriptionToServer()));
    connect(_controllerChat->getViewEdition(), SIGNAL(requestEditUser()), this, SLOT(editUser()));
    connect(cc, SIGNAL(binaryMessageReceived(const QByteArray&)), i, SLOT(processData(const QByteArray&)));
    connect(_view->getViewInscription(), SIGNAL(requestCancelInscription()), this, SLOT(cancelInscription()));
}

ControllerUser::~ControllerUser()
{
    delete _view;
}


void ControllerUser::showView() const
{
    _view->show();
}


void ControllerUser::connectToServer(bool fromBtnConnection)
{
    this->_fromBtnConnection = fromBtnConnection;

    if(!_connected)
    {
        // Use the getter to retrieve the data
        QString server = _view->getIpAddress();
        QString port = _view->getPort();   //

        // Connection to the servers
        _cc->connectToServer(server + ":" + port);
        _connected = true;
    }
    // if the user has already a socket open with the server, no need to
    // open a connection again. We can skip to next step.
    else
    {
        auth();
    }
}

void ControllerUser::auth() const
{
    if(_fromBtnConnection)
    {
        qDebug() << "Demande du sel";
        // Disabling the view and retrieving of "passwordSalt"
        // The salt will be received in the slot receiveSalt
        _view->setDisabled(true);
        _co->askForSalt(_view->getUsername());
    }
    else  // button inscription
    {
        // Open the inscription window
        _view->getViewInscription()->show();
        // Disable the main windows
        _view->setEnabled(false);
        _view->getViewInscription()->setEnabled(true);
    }
}

void ControllerUser::receiveSalt(const Salt& salt) const
{
    QByteArray tmp((const char*) salt.data(), salt.size());
    qDebug() << "Reception du sel: " << QString::fromUtf8(tmp.toHex());
    Hash hashPassword = _cryptor->generateHash(_view->getPassword().toStdString(), salt);

    _co->login(_view->getUsername(), hashPassword);
}

void ControllerUser::infoUser(ModelUser& user, const Salt& keySalt, RSAPair& rsaKeys) {
    
    _model->addUser(user);
    *_currentUser = _model->getUser(user.getIdUser());
    
    QString password (_view->getViewInscription()->getPassword());
    if (password.isEmpty())
    {
        _cryptor->decryptWithAES(rsaKeys,  _cryptor->generateAESKeyFromHash(_cryptor->generateHash(_view->getPassword().toStdString(), keySalt)));
    }
    
    else
    {
        _cryptor->decryptWithAES(rsaKeys,  _cryptor->generateAESKeyFromHash(_cryptor->generateHash(password.toStdString(), keySalt)));
    }
    
    _model->setRsaKeyPair(rsaKeys);
    qDebug() << "Affichage de l'interface prinicpale";
    _controllerChat->showView();
    _view->close();
    qDebug() << "Utilisateur Reçu !";
    
}

void ControllerUser::inscriptionToServer() const
{
    qDebug() << "Inscription to server";
    // Get user information
    const QString firstName = _view->getViewInscription()->getFirstName();
    const QString lastName = _view->getViewInscription()->getLastName();
    const QString userName = _view->getViewInscription()->getUserName();
    const QImage profilePicture = _view->getViewInscription()->getProfileImage();
    string password = _view->getViewInscription()->getPassword().toStdString();

    Salt passwordSalt = _cryptor->generateSalt();
    Hash hashPassword = _cryptor->generateHash(password, passwordSalt);
    RSAPair keyPair = _cryptor->generateRSAPair();
    Salt keySalt = _cryptor->generateSalt();
    
    QByteArray tmp((const char*) keyPair.publicKey.data(), keyPair.publicKey.size());
    qDebug() << "Clé publique : " << QString::fromUtf8(tmp.toHex());
    
    _cryptor->encryptWithAES(keyPair, _cryptor->generateAESKeyFromHash(_cryptor->generateHash(password, keySalt)));

    //Store information into a ModelUser
    ModelUser myUser(0, userName, firstName, lastName, false,  QDateTime::currentDateTime(), profilePicture, QSet<quint32>());

    //send data to the server
    _co->createAccount(myUser, hashPassword, passwordSalt, keySalt, keyPair);
}

void ControllerUser::editUser() const
{
    const QString firstName = _controllerChat->getViewEdition()->getFirstName();
    const QString lastName = _controllerChat->getViewEdition()->getLastName();
    QString previousPassword = _controllerChat->getViewEdition()->getPassword();
    const QImage profilePicture = _controllerChat->getViewEdition()->getProfileImage();

    ModelUser myUser(_currentUser->getIdUser(), "", firstName, lastName, false,  QDateTime::currentDateTime(), profilePicture, QSet<quint32>());

    //Modification are sent to the server
    _co->editAccount(myUser);

    //Local current user is updated
    _currentUser->setFirstName(firstName);
    _currentUser->setLastName(lastName);
}

void ControllerUser::cancelInscription()
{
    _fromBtnConnection = true;
    _view->setEnabled(true);
    _view->setInfoText("");
}
