HEADERS       = server.h
SOURCES       = server.cpp \
                main.cpp
QT           += network widgets

# install
target.path = $$[QT_INSTALL_EXAMPLES]/corelib/ipc/localfortuneserver
INSTALLS += target

DESTDIR         = $$PWD/../../app_bin/ipc
MOC_DIR         = $$PWD/../../build/ipc/localfortuneserver
OBJECTS_DIR     = $$PWD/../../build/ipc/localfortuneserver
