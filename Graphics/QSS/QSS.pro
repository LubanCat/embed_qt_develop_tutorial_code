#-------------------------------------------------
#
# Project created by QtCreator 2020-01-19T09:49:00
#
#-------------------------------------------------

########################################################################################################
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QSS
TEMPLATE = app
DESTDIR  = $$PWD/../../../bin
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG -= debug_and_release
CONFIG += c++11
SOURCES += \
        main.cpp \
        mainwindow.cpp
HEADERS += \
        mainwindow.h
FORMS += \
        mainwindow.ui
RESOURCES += \
        image.qrc


DESTDIR         = $$PWD/../../app_bin/graphics
MOC_DIR         = $$PWD/../../build/graphics/qss
OBJECTS_DIR     = $$PWD/../../build/graphics/qss
