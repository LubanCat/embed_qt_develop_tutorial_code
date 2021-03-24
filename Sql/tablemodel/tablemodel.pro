HEADERS       = ../connection.h
SOURCES       = tablemodel.cpp
QT           += sql widgets
requires(qtConfig(tableview))

# install
target.path = $$[QT_INSTALL_EXAMPLES]/sql/tablemodel
INSTALLS += target

DESTDIR         = $$PWD/../../app_bin/sql
MOC_DIR         = $$PWD/../../build/sql/tablemodel
OBJECTS_DIR     = $$PWD/../../build/sql/tablemodel
