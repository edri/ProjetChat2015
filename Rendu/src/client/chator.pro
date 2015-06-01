

QT       += core gui websockets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Chator
TEMPLATE = app

CONFIG += c++11

SOURCES += roomModule/controllerRoom.cpp roomModule/viewRoom.cpp roomModule/viewJoin.cpp ../common/cryptor/cryptor.cpp

HEADERS  += roomModule/controllerRoom.h roomModule/viewRoom.h roomModule/viewJoin.h ../common/cryptor/cryptor.h

FORMS    += 

RESOURCES += \
    icons.qrc

win32 {
    LIBS += -LC:/OpenSSL-Win32/lib -llibeay32
    INCLUDEPATH += C:/OpenSSL-Win32/include
}

linux-g++* {
    LIBS += -lcrypto
}
