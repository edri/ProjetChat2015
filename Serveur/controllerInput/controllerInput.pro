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
    modelChator.cpp

HEADERS += \
    controllerInput.h \
    modelChator.h