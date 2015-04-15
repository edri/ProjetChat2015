#ifndef INTERPRETOR_H
#define INTERPRETOR_H

#include "modelChator.h"
#include <QObject>

class Interpretor : public QObject
{
    public:
    void sendMessage(const ModelMessage& message);
    
    //private:
};

#endif
