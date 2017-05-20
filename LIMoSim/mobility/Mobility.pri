include(followerModel/FollowerModels.pri)
include(laneChange/LaneChangeModels.pri)
include(routing/Routing.pri)
include(trafficRules/TrafficRules.pri)

HEADERS += \
    $$PWD/mobilitymodel.h \
    $$PWD/randomdestination.h \
    $$PWD/mobilitymodels.h \
    $$PWD/randomneighbor.h \
    $$PWD/followlane.h \
    $$PWD/roadperception.h \
    $$PWD/followpath.h \
    $$PWD/alignmentcontrol.h \
    $$PWD/trip.h

SOURCES += \
    $$PWD/mobilitymodel.cc \
    $$PWD/randomdestination.cc \
    $$PWD/randomneighbor.cc \
    $$PWD/followlane.cc \
    $$PWD/roadperception.cc \
    $$PWD/followpath.cc \
    $$PWD/alignmentcontrol.cc \
    $$PWD/trip.cc
