TEMPLATE        = subdirs
SUBDIRS         = googlepinyin plugin testWindow NotePad \
    sysInput

CONFIG          += ordered

testWindow.depends  = plugin
plugin.depends  = googlepinyin
