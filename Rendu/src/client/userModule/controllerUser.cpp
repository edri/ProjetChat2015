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
    connect(_view->getViewInscription(), SIGNAL(requestGetNewUser()), this, SLOT(checkUsername()));
    connect(cc, SIGNAL(binaryMessageReceived(const QByteArray&)), i, SLOT(processData(const QByteArray&)));
    connect(_view->getViewInscription(), SIGNAL(requestCancelInscription()), this, SLOT(cancelInscription()));
    connect(cc, SIGNAL(disconnected()), SLOT(serverDisconnected()));
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
    }
    // if the user has already a socket open with the server, no need to
    // open a connection again. We can skip to next step.
    else
    {
        auth();
    }
}

void ControllerUser::auth()
{
    // we can stop the timeout timer
    _view->stopTimer();
    _connected = true;

    if(_fromBtnConnection)  //connection's button
    {
        qDebug() << "Demande du sel";
        // Disabling the view and retrieving of "passwordSalt"
        // The salt will be received in the slot receiveSalt
        _view->setDisabled(true);
        _co->askForSalt(_view->getUsername());
    }
    else  // inscription's button
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
    //Create a temporal QByteArray to store the salt
    QByteArray tmp((const char*) salt.data(), salt.size());

    // Hash's generation from the password
    Hash hashPassword = _cryptor->generateHash(_view->getPassword().toStdString(), salt);

    // Send username and hash to prove user's identity
    _co->login(_view->getUsername(), hashPassword);
}

void ControllerUser::infoUser(ModelUser& user, const Salt& keySalt, RSAPair& rsaKeys)
{
    // Receive user's information from the server and store it in the model
    _model->addUser(user);
    *_currentUser = _model->getUser(user.getIdUser());
    
    QString password (_view->getViewInscription()->getPassword());

    // Decrypt the private key in order to to decrypt the future received messages
    if (password.isEmpty())
    {
        _cryptor->decryptWithAES(rsaKeys,  _cryptor->generateAESKeyFromHash(_cryptor->generateHash(_view->getPassword().toStdString(), keySalt)));
    }
    else
    {
        _cryptor->decryptWithAES(rsaKeys,  _cryptor->generateAESKeyFromHash(_cryptor->generateHash(password.toStdString(), keySalt)));
    }
    
    // Configure the key pair in the local model
    _model->setRsaKeyPair(rsaKeys);

    // Show the main chat window
    _controllerChat->showView();
    _view->close();
}

void ControllerUser::usernameResponse(const bool exists) const
{
    if (exists)
    {
        // Show an error if user already exist
        _view->getViewInscription()->usernameAlreadyExistd();
    }
    else
    {
        // user can is allow to create his account
        inscriptionToServer();
    }
}

void ControllerUser::inscriptionToServer() const
{
    // Fetch user's information
    const QString firstName = _view->getViewInscription()->getFirstName();
    const QString lastName = _view->getViewInscription()->getLastName();
    const QString userName = _view->getViewInscription()->getUserName();
    const QImage profilePicture = _view->getViewInscription()->getProfileImage();
    string password = _view->getViewInscription()->getPassword().toStdString();

    // Salt generation and hash
    Salt passwordSalt = _cryptor->generateSalt();
    Hash hashPassword = _cryptor->generateHash(password, passwordSalt);
    RSAPair keyPair = _cryptor->generateRSAPair();
    Salt keySalt = _cryptor->generateSalt();

    QByteArray tmp((const char*) keyPair.publicKey.data(), keyPair.publicKey.size());

    // Encrypt private key in order to store in the server's database
    _cryptor->encryptWithAES(keyPair, _cryptor->generateAESKeyFromHash(_cryptor->generateHash(password, keySalt)));

    // Store information into a ModelUser
    ModelUser myUser(0, userName, firstName, lastName, false,  QDateTime::currentDateTime(), profilePicture, QSet<quint32>());

    // Send data to the server
    _co->createAccount(myUser, hashPassword, passwordSalt, keySalt, keyPair);
}

void ControllerUser::checkUsername() const
{
    // Request the server to know if the userName already exists.
    _co->userId(_view->getViewInscription()->getUserName());
}

void ControllerUser::cancelInscription()
{
    _fromBtnConnection = true;
    _view->setEnabled(true);
    _view->setInfoText("");
}

void ControllerUser::authError()
{
    _view->authError();
}

void ControllerUser::serverDisconnected()
{
    _connected = false;
}
