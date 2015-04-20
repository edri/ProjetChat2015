CONFIG += c++11

QT += core websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
TARGET = server
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += listener.h connector.h
SOURCES += main.cpp listener.cpp connector.cpp
