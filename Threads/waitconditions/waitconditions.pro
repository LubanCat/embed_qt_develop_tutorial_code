QT = core
CONFIG -= moc app_bundle
CONFIG += console

SOURCES += waitconditions.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/corelib/threads/waitconditions
INSTALLS += target

DESTDIR         = $$PWD/../../app_bin/threads
MOC_DIR         = $$PWD/../../build/threads/waitconditions
OBJECTS_DIR     = $$PWD/../../build/threads/waitconditions
