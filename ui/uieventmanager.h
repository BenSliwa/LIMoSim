#ifndef UIEVENTMANAGER_H
#define UIEVENTMANAGER_H

#include <QObject>
#include <QDebug>

namespace LIMOSIM
{
class CarUi;
class Node;
class NodeUi;
class SegmentMarker;

class UiEventManager : public QObject
{
    Q_OBJECT
public:
    UiEventManager(QObject *_parent = 0);

    static UiEventManager* getInstance();

    void route(NodeUi *_from, NodeUi *_to);

    void handleSegmentSelection(SegmentMarker *_segment);
    void handleCarSelection(CarUi *_car);


signals:
    void nodeMoved(Node *_node);
    void display(const QString &_text);
    void segmentSelected(SegmentMarker *_segment);
    void carSelected(CarUi *_car);

private slots:
    void onNodeMoved();
    void onNodeLeftClicked();
    void onNodeRightClicked();
    void onNodeHovered();
    void onNodeUnhovered();

private:
    NodeUi *p_start;
    NodeUi *p_destination;



};



}

#endif // UIEVENTMANAGER_H
