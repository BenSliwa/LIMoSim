QT += qml quick
CONFIG += c++11

SOURCES += main.cc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

INCLUDEPATH += $$PWD/../core
INCLUDEPATH += $$PWD/../ # project view from core

#LIBS += -lxml

include(../core/Core.pri)
include(Ui.pri)
include(../resources/Resources.pri)

RESOURCES +=

DISTFILES +=

