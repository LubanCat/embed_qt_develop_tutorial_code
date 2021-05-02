INCLUDEPATH     += $$PWD
DEPENDPATH      += $$PWD

INCLUDEPATH     += $$PWD/qqlist
DEPENDPATH      += $$PWD/qqlist $$PWD/picture


HEADERS += \
    $$PWD/animationstackedwidget.h \
    $$PWD/chatbubble.h \
    $$PWD/customwidget.h \
    $$PWD/qqlist/qqlistwidget.h \
    $$PWD/qqlist/qqlistviewgroup.h \
    $$PWD/qqlist/qqlistviewchild.h \
    $$PWD/qqlist/qqpopmenuwidget.h \
    $$PWD/rotatingstackedwidget.h \
    $$PWD/clineedit.h

SOURCES += \
    $$PWD/animationstackedwidget.cpp \
    $$PWD/chatbubble.cpp \
    $$PWD/customwidget.cpp \
    $$PWD/qqlist/qqlistwidget.cpp \
    $$PWD/qqlist/qqlistviewgroup.cpp \
    $$PWD/qqlist/qqlistviewchild.cpp \
    $$PWD/qqlist/qqpopmenuwidget.cpp \
    $$PWD/rotatingstackedwidget.cpp \
    $$PWD/clineedit.cpp


FORMS += \
    $$PWD/qqlist/qqlistviewgroup.ui
