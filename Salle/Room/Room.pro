#-------------------------------------------------
#
# Project created by QtCreator 2012-10-23T09:10:11
#
#-------------------------------------------------

CONFIG += c++11
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = room
TEMPLATE = app

SOURCES += main.cpp\
        controllerRoom.cpp\
        viewRoom.cpp\
        ../../ModeleChator/modelChator.cpp
        

HEADERS  += controllerRoom.h\
         viewRoom.h
         
