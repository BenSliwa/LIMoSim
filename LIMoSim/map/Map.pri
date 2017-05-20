include(road/Roads.pri)
include(osm/OSM.pri)

HEADERS += \
    $$PWD/node.h \
    $$PWD/way.h \
    $$PWD/map.h \
    $$PWD/mapobject.h \
    $$PWD/mapdefinitions.h \
    $$PWD/segment.h \
    $$PWD/lane.h \
    $$PWD/nodegrid.h \
    $$PWD/trafficsignal.h \
    $$PWD/lanegate.h

SOURCES += \
    $$PWD/node.cc \
    $$PWD/way.cc \
    $$PWD/map.cc \
    $$PWD/mapobject.cc \
    $$PWD/segment.cc \
    $$PWD/lane.cc \
    $$PWD/nodegrid.cc \
    $$PWD/trafficsignal.cc \
    $$PWD/lanegate.cc
