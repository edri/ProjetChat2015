CONFIG += c++11 console

QT += core websockets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEPENDPATH += .
INCLUDEPATH += .

win32 {
    TARGET = ../../bin/server/Windows/chatorServer
    LIBS += -LC:/OpenSSL-Win32/lib -llibeay32
    INCLUDEPATH += C:/OpenSSL-Win32/include
}

linux-g++* {
    QMAKE_CXXFLAGS += -Wextra -Wconversion -pedantic
    TARGET = ../../bin/server/Linux/chatorServer
    LIBS += -lcrypto
}


# Input

HEADERS += ../common/interpretor/interpretor.h ../common/models/packet.h ../common/models/modelChator.h ../common/controllerInput/controllerInput.h utils/listener.h controllers/controllerDB.h controllers/controllerUser.h controllers/controllerRoom.h controllers/serverControllerInput.h utils/serverModel.h ../common/models/chatorConstants.h ../common/cryptor/cryptor.h
SOURCES += ../common/interpretor/interpretor.cpp chatorServer.cpp ../common/models/modelChator.cpp utils/listener.cpp controllers/controllerDB.cpp controllers/controllerUser.cpp controllers/controllerRoom.cpp controllers/serverControllerInput.cpp utils/serverModel.cpp ../common/cryptor/cryptor.cpp
