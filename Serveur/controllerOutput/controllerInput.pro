CONFIG += c++11

QT += core gui websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = controllerOutput
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

HEADERS += test.h ../ServerConnector/connector.h ../Interpretor/interpretor.h ../Interpretor/packet.h ../../ModeleChator/modelChator.h ../controllerInput/controllerInput.h controllerOutput.h
SOURCES += test.cpp ../ServerConnector/connector.cpp ../Interpretor/interpretor.cpp main.cpp ../../ModeleChator/modelChator.cpp ../Interpretor/packet.cpp ../controllerInput/controllerInput.cpp controllerOutput.cpp
