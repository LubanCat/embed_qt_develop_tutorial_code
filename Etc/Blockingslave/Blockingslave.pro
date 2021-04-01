QT += widgets serialport
requires(qtConfig(combobox))

TARGET = Blockingslave
TEMPLATE = app

HEADERS += \
    dialog.h \
    slavethread.h

SOURCES += \
    main.cpp \
    dialog.cpp \
    slavethread.cpp

#target.path = $$[QT_INSTALL_EXAMPLES]/serialport/blockingslave
#INSTALLS += target


DESTDIR         = $$PWD/../app_bin
MOC_DIR         = $$PWD/../build/blockingslave
OBJECTS_DIR     = $$PWD/../build/blockingslave
