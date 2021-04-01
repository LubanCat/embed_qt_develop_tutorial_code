QT += widgets serialport
requires(qtConfig(combobox))

TARGET = Blockingmaster
TEMPLATE = app

HEADERS += \
    dialog.h \
    masterthread.h

SOURCES += \
    main.cpp \
    dialog.cpp \
    masterthread.cpp

#target.path = $$[QT_INSTALL_EXAMPLES]/serialport/blockingmaster
#INSTALLS += target

DESTDIR         = $$PWD/../app_bin
MOC_DIR         = $$PWD/../build/blockingmaster
OBJECTS_DIR     = $$PWD/../build/blockingmaster
