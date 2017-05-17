#ifndef LIMOSIM_NODEUI_H
#define LIMOSIM_NODEUI_H

#include "mapelementui.h"
#include "core/map/node.h"
#include <QList>

namespace LIMoSim
{

class NodeUi : public MapElementUi
{
    Q_OBJECT
public:
    NodeUi(QQuickItem *_parent = 0);

    QList<NodeUi*> updateDestinations();
    bool isVisible();

    // property accessors
    void setNode(Node *_node);

    Node* getNode();

    void handleMovement();

    // EPS export
    QString toEps();
    QString exportLights(const QString &_top, const QString &_center, const QString &_bottom);
    QString exportLight(int _index, const QString &_color);

private:
    void drawLights(const QColor &_top, const QColor &_center, const QColor &_bottom);
    void drawLight(int _index, const QColor &_color);



protected:
    void handlePaintEvent();
    void mousePressEvent(QMouseEvent *_event);
    void mouseReleaseEvent(QMouseEvent *_event);
    void mouseMoveEvent(QMouseEvent *_event);
    void hoverEnterEvent(QHoverEvent *_event);
    void hoverLeaveEvent(QHoverEvent *_event);


signals:
    void moved();
    void leftClicked();
    void rightClicked();
    void hovered();
    void unhovered();

private:
    Node *p_node;
    bool m_dragActive;


};

}

#endif // LIMOSIM_NODEUI_H
