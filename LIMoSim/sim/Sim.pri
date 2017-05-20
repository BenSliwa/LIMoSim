include(statistics/Statistics.pri)

HEADERS += \
    $$PWD/simulation.h \
    $$PWD/eventscheduler.h \
    $$PWD/event.h \
    $$PWD/eventhandler.h \
    $$PWD/rng.h \
    $$PWD/simulationdefinitions.h

SOURCES += \
    $$PWD/simulation.cc \
    $$PWD/eventscheduler.cc \
    $$PWD/event.cc \
    $$PWD/eventhandler.cc \
    $$PWD/rng.cc
