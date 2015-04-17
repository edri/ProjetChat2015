#-------------------------------------------------
#
# Project created by QtCreator 2015-04-16T08:57:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Chat
TEMPLATE = app


SOURCES += main.cpp\
        viewChat.cpp \
    controllerChat.cpp \
    "../ModeleChator/modelChator.cpp"

HEADERS  += viewChat.h \
    controllerChat.h \
    "../ModeleChator/modelChator.h"

FORMS    += viewChat.ui

RESOURCES += \
    icons.qrc
