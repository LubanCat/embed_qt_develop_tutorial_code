greaterThan(QT_MAJOR_VERSION, 4): QT += multimedia


HEADERS += \
    $$PWD/voice.h \
    $$PWD/test/testmedia.h \
    $$PWD/AudioRecorder.h

SOURCES += \
    $$PWD/voice.cpp \
    $$PWD/test/testmedia.cpp \
    $$PWD/AudioRecorder.cpp

INCLUDEPATH     += $$PWD
INCLUDEPATH     += $$PWD/test

INCLUDEPATH     += $$PWD/voice/include

INCLUDEPATH     += $$PWD/fmod/include

LIBS            += -L$$PWD/voice/libs -lmsc
LIBS            += -L$$PWD/fmod/libs -lfmode

win32 {
#LIBS            += -L$$PWD/fmod/libs -lfmodex
}
unix {
#LIBS            += $$PWD/fmod/libs/libfmodex.a
#LIBS            += -L$$PWD/voice/libs -lmsc
}

#QMAKE_POST_LINK += $$quote(cmd /c copy /y .\media\voice\libs\msc.dll .\bin)
#QMAKE_POST_LINK += $$quote(cmd /c copy /y .\media\fmod\libs\fmodex.dll .\bin)
