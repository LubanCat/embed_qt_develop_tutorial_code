QT          -= gui

TARGET      = googlepinyin
TEMPLATE    = lib
CONFIG      += staticlib

SOURCES     += \
            dictbuilder.cpp \
            dictlist.cpp \
            dicttrie.cpp \
            lpicache.cpp \
            matrixsearch.cpp \
            mystdlib.cpp \
            ngram.cpp \
            pinyinime.cpp \
            searchutility.cpp \
            spellingtable.cpp \
            spellingtrie.cpp \
            splparser.cpp \
            sync.cpp \
            userdict.cpp \
            utf16char.cpp \
            utf16reader.cpp

HEADERS     += \
            atomdictbase.h \
            dictbuilder.h \
            dictdef.h \
            dictlist.h \
            dicttrie.h \
            lpicache.h \
            matrixsearch.h \
            mystdlib.h \
            ngram.h \
            pinyinime.h \
            searchutility.h \
            spellingtable.h \
            spellingtrie.h \
            splparser.h \
            sync.h \
            userdict.h \
            utf16char.h \
            utf16reader.h

win32{
    CONFIG      += debug_and_release build_all
    CONFIG(debug, debug|release){
        TARGET  = ../../plugin/googlepinyin/$$join(TARGET,,,d)
    }CONFIG(release, debug|release){
        TARGET  = ../../plugin/googlepinyin/$$TARGET
    }
}
#unix{
##    TARGET      = ../plugin/googlepinyin/$$TARGET
#    TARGET      = $$PWD/../plugin/googlepinyin/$$TARGET
#    MOC_DIR     = ../tmpfiles
#    RCC_DIR     = ../tmpfiles
#    UI_DIR      = ../tmpfiles
#    OBJECTS_DIR = ../tmpfiles
#}

unix{
    DESTDIR         = $$PWD/../plugin/googlepinyin
    MOC_DIR         = $$PWD/../../build/input/googlepinyin
    OBJECTS_DIR     = $$PWD/../../build/input/googlepinyin
}
