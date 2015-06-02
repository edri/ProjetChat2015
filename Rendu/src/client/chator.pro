

QT       += core gui websockets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Chator
TEMPLATE = app

CONFIG += c++11

#QMAKE_CXXFLAGS += -Wall -Wextra -Wconversion -pedantic

SOURCES += chator.cpp roomModule/controllerRoom.cpp roomModule/viewRoom.cpp roomModule/viewJoin.cpp ../common/cryptor/cryptor.cpp userModule/controllerUser.cpp userModule/viewInscription.cpp userModule/viewUser.cpp connectionModule/controllerOutput.cpp connectionModule/connector.cpp connectionModule/clientControllerInput.cpp chatModule/controllerChat.cpp chatModule/viewabout.cpp chatModule/viewChat.cpp chatModule/viewmembershiprequests.cpp ../common/interpretor/interpretor.cpp ../common/models/modelChator.cpp

HEADERS  += roomModule/controllerRoom.h roomModule/viewRoom.h roomModule/viewJoin.h ../common/cryptor/cryptor.h userModule/viewUser.h userModule/viewInscription.h userModule/controllerUser.h connectionModule/controllerOutput.h connectionModule/connector.h connectionModule/clientControllerInput.h chatModule/controllerChat.h chatModule/viewabout.h chatModule/viewChat.h chatModule/viewmembershiprequests.h ../common/controllerInput/controllerInput.h ../common/interpretor/interpretor.h ../common/models/modelChator.h  ../common/models/packet.h ../common/models/chatorConstants.h

FORMS    += userModule/viewInscription.ui userModule/viewUser.ui chatModule/viewabout.ui chatModule/viewChat.ui chatModule/viewmembershiprequests.ui

RESOURCES += icons.qrc

win32 {
    LIBS += -LC:/OpenSSL-Win32/lib -llibeay32
    INCLUDEPATH += C:/OpenSSL-Win32/include
}

linux-g++* {
    LIBS += -lcrypto
}
