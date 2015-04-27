#-------------------------------------------------
#
# Project created by QtCreator 2012-10-23T09:10:11
#
#-------------------------------------------------

CONFIG += c++11
QT       += core gui websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = room
TEMPLATE = app

SOURCES += _main.cpp controllerRoom.cpp viewRoom.cpp ../Join/viewJoin.cpp ../../ModeleChator/modelChator.cpp ../../Serveur/Interpretor/packet.cpp ../../Serveur/Interpretor/interpretor.cpp ../../Serveur/ServerConnector/connector.cpp ../../Serveur/controllerInput/clientControllerInput.cpp ../../Serveur/controllerOutput/controllerOutput.cpp ../../Chat/controllerChat.cpp ../../Chat/viewChat.cpp
        

HEADERS  += controllerRoom.h viewRoom.h ../Join/viewJoin.h ../../ModeleChator/modelChator.h ../../Serveur/Interpretor/interpretor.h ../../Serveur/Interpretor/packet.h ../../Serveur/ServerConnector/connector.h ../../Serveur/controllerInput/controllerInput.h ../../Serveur/controllerInput/clientControllerInput.h ../../Serveur/controllerOutput/controllerOutput.h ../../Chat/controllerChat.h ../../Chat/viewChat.h

FORMS    += ../../Chat/viewChat.ui
         
