#include "controllerUser.h"

ControllerUser::ControllerUser(ClientControllerInput* cci, Interpretor* i, ClientConnector* cc,
                               ControllerOutput* co)
{
    this->cci = cci;
    this->i = i;
    this->cc = cc;
    this->co = co;
    // Initialize a new view for the connection
    this->_view = new ViewUser();

    // Bind the signals and the slots
    connect(_view, SIGNAL(requestGetIds()), this, SLOT(connectToServeur()));
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


void ControllerUser::connectToServeur() const
{
    // Use the getter to retrieve the data
    QString username = "Jean-paul";
    QString password = "fromage66";
    QString server = "localhost";
    QString port = "1234";   //

    // Connection to the servers
    cc->connectToServer("192.168.1.103:1234");
}

void ControllerUser::auth() const
{
    co->login("licorne", "java");
}

