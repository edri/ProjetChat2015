#include "test.h"

Test::Test()
{
    cci = new ClientControllerInput();
    i = new Interpretor(*cci);
    cc = new ClientConnector();
    
    connect(cc, SIGNAL(connectionSuccessful()), this, SLOT(auth()));
    
    co = new ControllerOutput(*cc, *i);
    cc->connectToServer("localhost:1234");
}

void Test::auth()
{
    co->login("Jean-paul", "fromage66");
}
