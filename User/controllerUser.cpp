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
    connect(_view->getViewInscription(), SIGNAL(requestGetNewUser()), this, SLOT(InscriptionToServer()));
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
        QString username = _view->getUsername();
        QString password = _view->getPassword();
        _co->login(username, password);
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


void ControllerUser::infoUser(ModelUser& user) {
    _model->addUser(user);
    *_currentUser = _model->getUser(user.getIdUser());
    _controllerChat->showView();
    _view->close();
}

void ControllerUser::InscriptionToServer() const
{
    //pour l'instant on récupère seulement username et password
    // Get user information
    QString firstName = _view->getViewInscription()->getFirstName();
    QString lastName = _view->getViewInscription()->getLastName();
    QString userName = _view->getViewInscription()->getUserName();
    QString password = _view->getViewInscription()->getPassword();
    QImage profilePicture = _view->getViewInscription()->getProfileImage();

    ModelUser myUser(0, userName, firstName, lastName, false,  QDateTime::currentDateTime(), profilePicture, QSet<quint32>());

    _co->createAccount(myUser, password);
}
