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
    _model = new ModelChator();
    _currentUser = currentUser;
    _controllerChat = controllerChat;

    // Bind the signals and the slots
    connect(_view, SIGNAL(requestGetIds(bool)), this, SLOT(connectToServer(bool)));
    connect(cc, SIGNAL(connectionSuccessful()), this, SLOT(auth()));
    connect(_view->getViewInscription(), SIGNAL(requestGetNewUser()), this, SLOT(inscriptionToServer()));
    connect(_controllerChat->getViewEdition(), SIGNAL(requestEditUser()), this, SLOT(editUser()));
    connect(cc, SIGNAL(binaryMessageReceived(const QByteArray&)), i, SLOT(processData(const QByteArray&)));
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
    // Use the getter to retrieve the data
    QString server = _view->getIpAddress();
    QString port = _view->getPort();   //

    this->_fromBtnConnection = fromBtnConnection;

    // Connection to the servers
    _cc->connectToServer(server + ":" + port);
}

void ControllerUser::auth() const
{
    if(_fromBtnConnection)
    {
        qDebug() << "Demande du sel";
        // Désactivation de la vue et Récupération de "passwordSalt"
        // le retour du sel se fera dans receiveSalt
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

void ControllerUser::infoUser(ModelUser& user) {
    _model->addUser(user);
    *_currentUser = _model->getUser(user.getIdUser());
    _controllerChat->showView();
    _view->close();
}

void ControllerUser::inscriptionToServer() const
{
    // Get user information
    const QString firstName = _view->getViewInscription()->getFirstName();
    const QString lastName = _view->getViewInscription()->getLastName();
    const QString userName = _view->getViewInscription()->getUserName();
    string password = _view->getViewInscription()->getPassword().toStdString();

    Salt passwordSalt = _cryptor->generateSalt();
    Hash hashPassword = _cryptor->generateHash(password, passwordSalt);
    RSAPair keyPair = _cryptor->generateRSAPair();
    Salt keySalt = _cryptor->generateSalt();
    //Hash keyHash = _cryptor->generateHash(password, keySalt);

    const QImage profilePicture = _view->getViewInscription()->getProfileImage();

    //Store information into a ModelUser
    ModelUser myUser(0, userName, firstName, lastName, false,  QDateTime::currentDateTime(), profilePicture, QSet<quint32>());

    //send data to the server
    _co->createAccount(myUser, hashPassword, passwordSalt, keySalt, keyPair);
}

void ControllerUser::editUser() const
{
    const QString firstName = _controllerChat->getViewEdition()->getFirstName();
    const QString lastName = _controllerChat->getViewEdition()->getLastName();
    //QString password = _controllerChat->getViewEdition()->getPassword();
    const QImage profilePicture = _controllerChat->getViewEdition()->getProfileImage();

    ModelUser myUser(_currentUser->getIdUser(), "", firstName, lastName, false,  QDateTime::currentDateTime(), profilePicture, QSet<quint32>());

    //Modification are sent to the server
    _co->editAccount(myUser);

    //Local current user is updated
    _currentUser->setFirstName(firstName);
    _currentUser->setLastName(lastName);
}
