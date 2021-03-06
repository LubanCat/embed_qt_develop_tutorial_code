HEADERS   = database.h \
            dialog.h \
            mainwindow.h
RESOURCES = masterdetail.qrc
SOURCES   = dialog.cpp \
            main.cpp \
            mainwindow.cpp

QT += sql widgets
QT += xml widgets
requires(qtConfig(tableview))

EXAMPLE_FILES = albumdetails.xml

# install
target.path = $$[QT_INSTALL_EXAMPLES]/sql/masterdetail
INSTALLS += target


DESTDIR         = $$PWD/../../app_bin/sql
MOC_DIR         = $$PWD/../../build/sql/masterdetail
OBJECTS_DIR     = $$PWD/../../build/sql/masterdetail
