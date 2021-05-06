HEADERS       = client.h
SOURCES       = client.cpp \
                main.cpp
QT           += network widgets

# install
target.path = $$[QT_INSTALL_EXAMPLES]/corelib/ipc/localfortuneclient
INSTALLS += target

DESTDIR         = $$PWD/../../app_bin/ipc
MOC_DIR         = $$PWD/../../build/ipc/localfortuneclient
OBJECTS_DIR     = $$PWD/../../build/ipc/localfortuneclient
