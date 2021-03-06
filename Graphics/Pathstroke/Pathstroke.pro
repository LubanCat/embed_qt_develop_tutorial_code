SOURCES += main.cpp pathstroke.cpp
HEADERS += pathstroke.h

SHARED_FOLDER = ../shared

include($$SHARED_FOLDER/shared.pri)

RESOURCES += pathstroke.qrc

qtHaveModule(opengl) {
	DEFINES += QT_OPENGL_SUPPORT
	QT += opengl
}
QT += widgets

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/painting/pathstroke
INSTALLS += target

DESTDIR         = $$PWD/../../app_bin/graphics
MOC_DIR         = $$PWD/../../build/graphics/pathstroke
OBJECTS_DIR     = $$PWD/../../build/graphics/pathstroke
