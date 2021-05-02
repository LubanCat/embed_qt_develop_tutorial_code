#-------------------------------------------------
#
# Project created by QtCreator 2017-04-24T11:08:48
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChatServer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    myapp.cpp \
    databasemagr.cpp \
    clientsocket.cpp \
    tcpserver.cpp

HEADERS  += mainwindow.h \
    myapp.h \
    databasemagr.h \
    clientsocket.h \
    tcpserver.h \
    unit.h \
    global.h

FORMS    += mainwindow.ui

RESOURCES += \
    images.qrc


include($$PWD/basewidget/basewidget.pri)

RC_FILE         = $$PWD/resource/ico/app.rc

#temp file
DESTDIR         = $$PWD/../../app_bin/chart
MOC_DIR         = $$PWD/../../build/chart/server
OBJECTS_DIR     = $$PWD/../../build/chart/server
DEFINES         += BUILD_BY_PRO


