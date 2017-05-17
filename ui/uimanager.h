#ifndef LIMOSIM_UIMANAGER_H
#define LIMOSIM_UIMANAGER_H

#include <QObject>
#include <QDebug>
#include <QMap>
#include <QQmlApplicationEngine>

#include "ui/map/viewport.h"
#include "ui/map/mapui.h"
#include "ui/uieventmanager.h"
#include "ui/map/uielements.h"

#include "core/sim/eventhandler.h"
#include "ui/editor/roadbuilder.h"

namespace LIMoSim
{

class UiManager : public QObject, EventHandler
{
    Q_OBJECT
public:
    UiManager(QObject *_parent = 0);
    ~UiManager();

    static UiManager* getInstance();

    void loadQml();

    Q_INVOKABLE bool handleDrop(const QString &_path);
    Q_INVOKABLE bool loadScenario(const QString &_path);
    Q_INVOKABLE bool saveScenario(const QString &_path);

    Q_INVOKABLE void generateUiElements();

    Q_INVOKABLE void clear();
    Q_INVOKABLE void exportScreenshot();


    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void step();
    Q_INVOKABLE void setTimeScaleFactor(double _factor);


    //
    NodeUi* getNodeUi(Node* _node);
    NodeUi* addNode(Node* _node);
    void removeNode(NodeUi* _node);
    void insertNode(Segment *_segment);

    //
    SegmentMarker* addSegment(Segment *_segment);
    void removeSegment(SegmentMarker *_segment);

    //
    CarUi* addCar(Car *_car);





    // property accessors
    MapUi* getMap();
    QMap<Node*, NodeUi*>& getNodes();
    QMap<Segment*, SegmentMarker*>& getSegments();
    QMap<Car*, CarUi*>& getCars();



protected:
    void handleEvent(Event *_event);

signals:
    void nodeRemoved(Node *_node);
    void segmentRemoved(Segment *_segment);
    void updated(int _events, int _scheduled, double _simTime_s, double _simDuration_s);

private:
    QQmlApplicationEngine m_qml;
    ViewPort *p_viewPort;
    MapUi *p_map;
    RoadBuilder *p_editor;

    UiEventManager m_uiEventManager;

    QMap<Node*, NodeUi*> m_nodes;
    QMap<Segment*, SegmentMarker*> m_segments;
    QMap<Car*, CarUi*> m_cars;

    Event *m_updateTimer;
    double m_updateInterval_s;
};

}


#endif // LIMOSIM_UIMANAGER_H
