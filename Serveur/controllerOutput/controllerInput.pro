CONFIG += c++11

QT += core gui websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = controllerOutput
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

HEADERS += connector.h interpretor.h packet.h modelChator.h controllerInput.h controllerOutput.h
SOURCES += connector.cpp interpretor.cpp main.cpp modelChator.cpp packet.cpp controllerInput.cpp controllerOutput.cpp
