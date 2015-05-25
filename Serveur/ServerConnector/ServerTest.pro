QT       += core websockets
QT       -= gui

TARGET = connector
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    connector.cpp

HEADERS += \
    connector.h
