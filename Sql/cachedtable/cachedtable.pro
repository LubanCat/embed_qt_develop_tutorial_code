QT           += sql widgets

HEADERS       = ../connection.h \
                tableeditor.h
SOURCES       = main.cpp \
                tableeditor.cpp
requires(qtConfig(tableview))

# install
target.path = $$[QT_INSTALL_EXAMPLES]/sql/cachedtable
INSTALLS += target

DESTDIR         = $$PWD/../../app_bin/sql
MOC_DIR         = $$PWD/../../build/sql/cachedtable
OBJECTS_DIR     = $$PWD/../../build/sql/cachedtable
