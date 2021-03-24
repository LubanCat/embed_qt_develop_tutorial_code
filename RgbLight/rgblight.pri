#-------------------------------------------------
# RGB-LED灯调节
#-------------------------------------------------

INCLUDEPATH     += $$PWD/src

HEADERS += \
    $$PWD/src/rgblightmonitor.h

SOURCES += \
    $$PWD/src/rgblightmonitor.cpp

INCLUDEPATH += $$PWD/../thirdpart/libqui/include
LIBS += -L$$PWD/../thirdpart/libqui/lib -lQtUi

INCLUDEPATH += $$PWD/../thirdpart/libskin/include
LIBS += -L$$PWD/../thirdpart/libskin/lib -lSkin

