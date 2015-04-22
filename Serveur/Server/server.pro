CONFIG += c++11 console

QT += core websockets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
TARGET = server
DEPENDPATH += .
INCLUDEPATH += .

# Input

HEADERS += connector.h interpretor.h packet.h modelChator.h controllerInput.h listener.h controllerDB.h controllerUser.h controllerRoom.h serverControllerInput.h
SOURCES += connector.cpp interpretor.cpp main.cpp modelChator.cpp packet.cpp controllerInput.cpp listener.cpp controllerDB.cpp controllerUser.cpp controllerRoom.cpp serverControllerInput.cpp
