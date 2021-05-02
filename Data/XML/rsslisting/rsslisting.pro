HEADERS += rsslisting.h
SOURCES += main.cpp rsslisting.cpp
QT += network xml widgets
requires(qtConfig(treewidget))

# install
target.path = $$[QT_INSTALL_EXAMPLES]/xml/rsslisting
INSTALLS += target

DESTDIR         = $$PWD/../../../app_bin/data/xml
MOC_DIR         = $$PWD/../../../build/data/xml/rsslisting
OBJECTS_DIR     = $$PWD/../../../build/data/xml/rsslisting
DEFINES         += BUILD_BY_PRO
