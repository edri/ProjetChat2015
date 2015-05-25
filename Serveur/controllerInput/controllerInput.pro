CONFIG += c++11

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = controllerInput
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    controllerInput.cpp \
    clientControllerInput.cpp \
    ../../ModeleChator/modelChator.cpp \
    ../../Chat/controllerchat.cpp \
    ../../Salle/Room/controllerRoom.cpp \
    ../../User/controllerUser.cpp

HEADERS += \
    controllerInput.h \
    clientControllerInput.h \
    ../../Chat/controllerchat.h \
    ../../Salle/Room/controllerRoom.h \
    ../../User/controllerUser.h \
    ../../ModeleChator/modelChator.h
