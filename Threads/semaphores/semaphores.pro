SOURCES += semaphores.cpp
QT = core

CONFIG -= app_bundle
CONFIG += console

# install
target.path = $$[QT_INSTALL_EXAMPLES]/corelib/threads/semaphores
INSTALLS += target

DESTDIR         = $$PWD/../../app_bin/threads
MOC_DIR         = $$PWD/../../build/threads/semaphores
OBJECTS_DIR     = $$PWD/../../build/threads/semaphores
