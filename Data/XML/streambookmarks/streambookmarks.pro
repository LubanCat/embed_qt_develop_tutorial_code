HEADERS       = mainwindow.h \
                xbelreader.h \
                xbelwriter.h
SOURCES       = main.cpp \
                mainwindow.cpp \
                xbelreader.cpp \
                xbelwriter.cpp
QT           += xml widgets
requires(qtConfig(filedialog))

EXAMPLE_FILES = frank.xbel jennifer.xbel

# install
target.path = $$[QT_INSTALL_EXAMPLES]/xml/streambookmarks
INSTALLS += target

DESTDIR         = $$PWD/../../../app_bin/data/xml
MOC_DIR         = $$PWD/../../../build/data/xml/streambookmarks
OBJECTS_DIR     = $$PWD/../../../build/data/xml/streambookmarks
DEFINES         += BUILD_BY_PRO
