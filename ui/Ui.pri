include(deployment.pri)
include(editor/Editor.pri)
include(map/Map.pri)
include(plot/Plot.pri)
include(sim/Sim.pri)
include(qml/Qml.pri)

HEADERS += \
    $$PWD/uimanager.h \
    $$PWD/uieventmanager.h \
    $$PWD/uisettings.h \
    $$PWD/qteventscheduler.h

SOURCES += \
    $$PWD/uimanager.cc \
    $$PWD/uieventmanager.cc \
    $$PWD/uisettings.cc \
    $$PWD/qteventscheduler.cc
