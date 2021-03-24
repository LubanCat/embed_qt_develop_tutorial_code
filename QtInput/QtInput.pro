TEMPLATE        = subdirs
SUBDIRS         = googlepinyin plugin testWindow NotePad

CONFIG          += ordered

testWindow.depends  = plugin
plugin.depends  = googlepinyin
