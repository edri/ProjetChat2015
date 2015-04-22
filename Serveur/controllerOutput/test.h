#ifndef TEST_CLASSE_DE_LA_MORT
#define TEST_CLASSE_DE_LA_MORT

#include <QObject>
#include "../controllerInput/controllerInput.h"
#include "../Interpretor/interpretor.h"
#include "../ServerConnector/connector.h"
#include "controllerOutput.h"

class Test : public QObject
{
    Q_OBJECT
    
    public:
    Test();
    
    public slots:
    void auth();
    
    private:
    ClientControllerInput *cci;
    Interpretor* i;
    ClientConnector* cc;
    ControllerOutput* co;
};

#endif
