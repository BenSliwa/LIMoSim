#include <QGuiApplication>
#include "LIMoSim/sim/simulation.h"
#include "ui/uimanager.h"
#include "LIMoSim/location/position.h"

#include <QFile>
#include <QDebug>

using namespace LIMoSim;

#include "LIMoSim/settings/filehandler.h"
#include "LIMoSim/settings/xmlparser.h"
#include "LIMoSim/location/wgs84.h"


#include "LIMoSim/sim/rng.h"

#include "ui/plot/box.h"
#include "ui/qteventscheduler.h"

#include <iostream>


void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if(msg!="QTransform::translate with NaN called")
        std::cout << msg.toStdString() << std::endl;
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);


    QtEventScheduler scheduler;
    Simulation *sim = Simulation::getInstance(&scheduler);


    /*************************************************
     *                   ADJUST THIS !
    **************************************************/
    std::string dir = "/home/andras/projects/LIMoSim/resources";

    std::string mapFile;
    std::string vehiclesFile;

    if (argc == 2)
        dir = argv[1];
    if (argc == 1 || argc == 2) {
        mapFile = dir + "/TUDO.osm";
        vehiclesFile = dir + "/Cars.xml";
    }
    else if (argc >= 3) {
        mapFile = argv[1];
        vehiclesFile = argv[2];
    }

    WGS84 converter;
    sim->load(mapFile, vehiclesFile, converter);



    UiManager *ui = UiManager::getInstance();


    return app.exec();
}
