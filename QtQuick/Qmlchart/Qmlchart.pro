QT += charts qml quick

TARGET = Qmlchart
TEMPLATE = app

SOURCES += \
    main.cpp

RESOURCES += \
    resources.qrc

DISTFILES += \
    qml/qmlchart/*

target.path = $$[QT_INSTALL_EXAMPLES]/charts/qmlchart
INSTALLS += target


DESTDIR         = $$PWD/../../app_bin/quick
MOC_DIR         = $$PWD/../../build/quick/qchart
OBJECTS_DIR     = $$PWD/../../build/quick/qchart
