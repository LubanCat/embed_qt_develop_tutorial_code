QT += widgets
requires(qtConfig(filedialog))

SOURCES += main.cpp \
           dialog.cpp 

HEADERS += dialog.h

# Forms and resources
FORMS += dialog.ui

EXAMPLE_FILES = *.png

# install
target.path = $$[QT_INSTALL_EXAMPLES]/corelib/ipc/sharedmemory
INSTALLS += target


DESTDIR         = $$PWD/../../app_bin/ipc
MOC_DIR         = $$PWD/../../build/ipc/sharedmemory
OBJECTS_DIR     = $$PWD/../../build/ipc/sharedmemory
