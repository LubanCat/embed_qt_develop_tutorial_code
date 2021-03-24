HEADERS       = ../connection.h \
                customsqlmodel.h \
                editablesqlmodel.h
SOURCES       = customsqlmodel.cpp \
                editablesqlmodel.cpp \
                main.cpp
QT           += sql widgets
requires(qtConfig(tableview))

# install
target.path = $$[QT_INSTALL_EXAMPLES]/sql/querymodel
INSTALLS += target

DESTDIR         = $$PWD/../../app_bin/sql
MOC_DIR         = $$PWD/../../build/sql/querymodel
OBJECTS_DIR     = $$PWD/../../build/sql/querymodel
