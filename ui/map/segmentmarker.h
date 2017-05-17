#ifndef SEGMENTMARKER_H
#define SEGMENTMARKER_H

#include "mapelementui.h"
#include "core/map/segment.h"

namespace LIMOSIM
{

class SegmentMarker : public MapElementUi
{
    Q_OBJECT

    Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(int lanes READ getLanes WRITE setLanes)
public:
    SegmentMarker(QQuickItem *_parent = 0);

    bool isVisible();

    // property accessors
    void setSegment(Segment *_segment);

    Segment* getSegment();

    // qml properties
    Q_INVOKABLE void setName(const QString &_name);
    Q_INVOKABLE void setLanes(int _lanes);

    Q_INVOKABLE QString getName();
    Q_INVOKABLE int getLanes();

    // EPS export
    QString toEps();

protected:
    void handlePaintEvent();
    void mousePressEvent(QMouseEvent *_event);
    void mouseReleaseEvent(QMouseEvent *_event);
    void hoverEnterEvent(QHoverEvent *_event);
    void hoverLeaveEvent(QHoverEvent *_event);

private slots:
    void onNodeMoved(Node *_node);
    void onNodeRemoved(Node *_node);
    void onSegmentRemoved(Segment *_segment);

signals:
    void leftClicked();

private:
    Segment *p_segment;
    Node *p_start;
    Node *p_end;

    Segment *p_predecessor;
    Segment *p_successor;

    QString text;
};


}

#endif // SEGMENTMARKER_H
