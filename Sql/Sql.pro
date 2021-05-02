requires(qtHaveModule(widgets))

TEMPLATE      = subdirs

SUBDIRS             =   books \
                        drilldown \
                        cachedtable \
                        querymodel \
                        relationaltablemodel \
                        sqlwidgetmapper \
                        tablemodel

qtHaveModule(xml): SUBDIRS += masterdetail

!cross_compile:{
    contains(QT_BUILD_PARTS, tools):{
        SUBDIRS += sqlbrowser
    }
}

EXAMPLE_FILES = connection.h

DESTDIR         = $$PWD/../app_bin/sql
MOC_DIR         = $$PWD/../build/sql
OBJECTS_DIR     = $$PWD/../build/sql
