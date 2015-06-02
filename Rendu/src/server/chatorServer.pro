CONFIG += c++11 console

QMAKE_CXXFLAGS += -Wall -Wextra -Wconversion -pedantic

QT += core websockets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
TARGET = chatorServer
DEPENDPATH += .
INCLUDEPATH += .

# Input

HEADERS += ../common/interpretor/interpretor.h ../common/models/packet.h ../common/models/modelChator.h ../common/controllerInput/controllerInput.h utils/listener.h controllers/controllerDB.h controllers/controllerUser.h controllers/controllerRoom.h controllers/serverControllerInput.h utils/serverModel.h ../common/models/chatorConstants.h ../common/cryptor/cryptor.h
SOURCES += ../common/interpretor/interpretor.cpp chatorServer.cpp ../common/models/modelChator.cpp utils/listener.cpp controllers/controllerDB.cpp controllers/controllerUser.cpp controllers/controllerRoom.cpp controllers/serverControllerInput.cpp utils/serverModel.cpp ../common/cryptor/cryptor.cpp

win32 {
    LIBS += -LC:/OpenSSL-Win32/lib -llibeay32
    INCLUDEPATH += C:/OpenSSL-Win32/include
}

linux-g++* {
    LIBS += -lcrypto
}
