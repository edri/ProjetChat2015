#include "controllerUser.h"

ControllerUser::ControllerUser(ModelChator* model, ModelUser* currentUser, ClientControllerInput* cci, Interpretor* i, ClientConnector* cc,
                               ControllerOutput* co, ControllerChat* controllerChat)
{
    this->cci = cci;
    this->i = i;
    this->cc = cc;
    this->co = co;
    // Initialize a new view for the connection
    this->_view = new ViewUser();
    this->_model = new ModelChator();
    this->_currentUser = currentUser;
    this->_controllerChat = controllerChat;

    // Bind the signals and the slots
    connect(_view, SIGNAL(requestGetIds()), this, SLOT(connectToServer()));
    connect(cc, SIGNAL(connectionSuccessful()), this, SLOT(auth()));
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


void ControllerUser::connectToServer() const
{
    // Use the getter to retrieve the data
    QString server = _view->getIpAddress();
    QString port = _view->getPort();   //

    // Connection to the servers
    cc->connectToServer(server + ":" + port);
}

void ControllerUser::auth() const
{
    QString username = _view->getUsername();
    QString password = _view->getPassword();
    co->login(username, password);
}


void ControllerUser::infoUser(ModelUser& user) {
    _model->addUser(user);
    *_currentUser = _model->getUser(user.getIdUser());
    _controllerChat->showView();
    _view->close();
}











