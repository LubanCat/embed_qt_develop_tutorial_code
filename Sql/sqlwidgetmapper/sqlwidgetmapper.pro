HEADERS   = window.h
SOURCES   = main.cpp \
            window.cpp
QT += sql widgets
requires(qtConfig(combobox))

# install
target.path = $$[QT_INSTALL_EXAMPLES]/sql/sqlwidgetmapper
INSTALLS += target


DESTDIR         = $$PWD/../../app_bin/sql
MOC_DIR         = $$PWD/../../build/sql/sqlbrowser
OBJECTS_DIR     = $$PWD/../../build/sql/sqlbrowser
