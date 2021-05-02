CONFIG   += cmdline
QT       -= gui
SOURCES  += main.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/xml/xmlstreamlint
INSTALLS += target

DESTDIR         = $$PWD/../../../app_bin/data/xml
MOC_DIR         = $$PWD/../../../build/data/xml/xmlstreamlint
OBJECTS_DIR     = $$PWD/../../../build/data/xml/xmlstreamlint
DEFINES         += BUILD_BY_PRO
