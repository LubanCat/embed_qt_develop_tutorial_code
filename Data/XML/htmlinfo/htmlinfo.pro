SOURCES += main.cpp
QT -= gui

RESOURCES   = resources.qrc

CONFIG += cmdline

# install
target.path = $$[QT_INSTALL_EXAMPLES]/xml/htmlinfo
INSTALLS += target

DESTDIR         = $$PWD/../../../app_bin/data/xml
MOC_DIR         = $$PWD/../../../build/data/xml/htmlinfo
OBJECTS_DIR     = $$PWD/../../../build/data/xml/htmlinfo
DEFINES         += BUILD_BY_PRO
