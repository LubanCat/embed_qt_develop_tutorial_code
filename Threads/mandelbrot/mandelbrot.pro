QT += widgets

HEADERS       = mandelbrotwidget.h \
                renderthread.h
SOURCES       = main.cpp \
                mandelbrotwidget.cpp \
                renderthread.cpp

unix:!mac:!vxworks:!integrity:!haiku:LIBS += -lm

# install
target.path = $$[QT_INSTALL_EXAMPLES]/corelib/threads/mandelbrot
INSTALLS += target

DESTDIR         = $$PWD/../../app_bin/threads
MOC_DIR         = $$PWD/../../build/threads/mandelbrot
OBJECTS_DIR     = $$PWD/../../build/threads/mandelbrot
