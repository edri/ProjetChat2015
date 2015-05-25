CONFIG += c++11 console

QT += core websockets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
TARGET = server
DEPENDPATH += .
INCLUDEPATH += .

# Input

HEADERS += ../Interpretor/interpretor.h ../Interpretor/packet.h ../../ModeleChator/modelChator.h ../controllerInput/controllerInput.h listener.h controllerDB.h controllerUser.h controllerRoom.h serverControllerInput.h serverModel.h ../../ModeleChator/chatorConstants.h ../../Cryptor/cryptor.cpp
SOURCES += ../Interpretor/interpretor.cpp main.cpp ../../ModeleChator/modelChator.cpp ../Interpretor/packet.cpp ../controllerInput/controllerInput.cpp listener.cpp controllerDB.cpp controllerUser.cpp controllerRoom.cpp serverControllerInput.cpp serverModel.cpp ../../Cryptor/cryptor.h

win32 {
    LIBS += -LC:/OpenSSL-Win32/lib -llibeay32
    INCLUDEPATH += C:/OpenSSL-Win32/include
}

linux-g++* {
    LIBS += -lcrypto
}
