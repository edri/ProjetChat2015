#-------------------------------------------------
#
# Project created by QtCreator 2015-04-18T19:16:32
#
#-------------------------------------------------

QT       += core gui websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = User
TEMPLATE = app

CONFIG += c++11


SOURCES += main.cpp\
    viewUser.cpp \
    controllerUser.cpp  \
    ../Serveur/ServerConnector/connector.cpp \
    ../Serveur/Interpretor/interpretor.cpp \
    ../Serveur/Interpretor/packet.cpp \
    ../ModeleChator/modelChator.cpp \
    ../Serveur/controllerInput/clientControllerInput.cpp \
    ../Serveur/controllerOutput/controllerOutput.cpp \
    ../Salle/Room/controllerRoom.cpp\
    ../Salle/Room/viewRoom.cpp\
    ../Salle/Join/viewJoin.cpp\
    viewInscription.cpp\
    ../Chat/controllerChat.cpp\
    ../Chat/viewChat.cpp \
    ../Cryptor/cryptor.cpp \
    ../Chat/viewmembershiprequests.cpp

HEADERS  += viewUser.h \
    controllerUser.h    \
    ../Serveur/ServerConnector/connector.h \
    ../Serveur/Interpretor/interpretor.h \
    ../Serveur/Interpretor/packet.h \
    ../ModeleChator/modelChator.h \
    ../Serveur/controllerInput/controllerInput.h \
    ../Serveur/controllerInput/clientControllerInput.h \
    ../Serveur/controllerOutput/controllerOutput.h \
    ../Salle/Room/controllerRoom.h\
    ../Salle/Room/viewRoom.h\
    ../Salle/Join/viewJoin.h\
    viewInscription.h\
    ../Chat/controllerChat.h\
    ../Chat/viewChat.h \
    ../Cryptor/cryptor.h \
    ../Chat/viewmembershiprequests.h

FORMS    += viewUser.ui \
    viewInscription.ui\
    ../Chat/viewChat.ui \
    ../Chat/viewmembershiprequests.ui

RESOURCES += \
    icons.qrc

win32 {
    LIBS += -LC:/OpenSSL-Win32/lib -llibeay32
    INCLUDEPATH += C:/OpenSSL-Win32/include
}

linux-g++* {
    LIBS += -lcrypto
}
