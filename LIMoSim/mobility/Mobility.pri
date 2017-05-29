include(followerModel/FollowerModels.pri)
include(laneChange/LaneChangeModels.pri)
include(routing/Routing.pri)
include(trafficRules/TrafficRules.pri)

HEADERS += \
    $$PWD/mobilitymodel.h \
    $$PWD/mobilitymodels.h \
    $$PWD/roadperception.h \
    $$PWD/followpath.h \
    $$PWD/alignmentcontrol.h \
    $$PWD/trip.h \
    $$PWD/randomdirection.h

SOURCES += \
    $$PWD/mobilitymodel.cc \
    $$PWD/roadperception.cc \
    $$PWD/followpath.cc \
    $$PWD/alignmentcontrol.cc \
    $$PWD/trip.cc \
    $$PWD/randomdirection.cc
