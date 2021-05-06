HEADERS   = block.h \
            renderthread.h \
            window.h
SOURCES   = main.cpp \
            block.cpp \
            renderthread.cpp \
            window.cpp
QT += widgets
requires(qtConfig(filedialog))

# install
target.path = $$[QT_INSTALL_EXAMPLES]/corelib/threads/queuedcustomtype
INSTALLS += target


DESTDIR         = $$PWD/../../app_bin/threads
MOC_DIR         = $$PWD/../../build/threads/queuedcustomtype
OBJECTS_DIR     = $$PWD/../../build/threads/queuedcustomtype
