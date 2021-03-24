HEADERS       = ../connection.h
SOURCES       = relationaltablemodel.cpp
QT           += sql widgets
requires(qtConfig(tableview))

# install
target.path = $$[QT_INSTALL_EXAMPLES]/sql/relationaltablemodel
INSTALLS += target

DESTDIR         = $$PWD/../../app_bin/sql
MOC_DIR         = $$PWD/../../build/sql/relationaltablemodel
OBJECTS_DIR     = $$PWD/../../build/sql/relationaltablemodel
