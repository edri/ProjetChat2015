#include "controllerUser.h"

ControllerUser::ControllerUser(ClientControllerInput* cci, Interpretor* i, ClientConnector* cc,
                               ControllerOutput* co)
{
    this->cci = cci;
    this->i = i;
    this->cc = cc;
    this->co = co;
    // Initialiser une nouvelle vue
    this->_view = new ViewUser();

    // Connecter le signal au slot
    connect(_view, SIGNAL(requestGetIds()), this, SLOT(connectToServeur()));
    connect(cc, SIGNAL(connectionSuccessful()), this, SLOT(auth()));


}

ControllerUser::~ControllerUser()
{

}


void ControllerUser::showView() const
{
    _view->show();
}


void ControllerUser::connectToServeur() const
{
    // Récupérer les données avec les getters
    QString username = "Jean-paul";
    QString password = "fromage66";
    QString server = "localhost";
    QString port = "1234";   //

    // Se connecter au serveur
    cc->connectToServer("localhost:1234");



}

void ControllerUser::auth() const
{
    co->login("Jean-paul", "fromage66");
}

