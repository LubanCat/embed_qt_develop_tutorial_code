HEADERS = ../connection.h \
          imageitem.h \
          informationwindow.h \
          view.h
RESOURCES = drilldown.qrc
SOURCES = imageitem.cpp \
          informationwindow.cpp \
          main.cpp \
          view.cpp
QT += sql widgets
requires(qtConfig(combobox))

# install
target.path = $$[QT_INSTALL_EXAMPLES]/sql/drilldown
INSTALLS += target

DESTDIR         = $$PWD/../../app_bin/sql
MOC_DIR         = $$PWD/../../build/sql/drilldown
OBJECTS_DIR     = $$PWD/../../build/sql/drilldown
