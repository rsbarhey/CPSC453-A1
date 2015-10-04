#-------------------------------------------------
#
# Project created by QtCreator 2015-09-16T16:15:03
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = A1
TEMPLATE = app
INCLUDEPATH += .

SOURCES += main.cpp\
        MainWindow.cpp \
    renderer.cpp \
    game.cpp \
    Cube.cpp \
    TCPServer.cpp \
    TCPClient.cpp

HEADERS  += MainWindow.h \
    renderer.h \
    game.h \
    Cube.h \
    TCPServer.h \
    TCPClient.h

FORMS    += MainWindow.ui

RESOURCES += \
    resources.qrc
