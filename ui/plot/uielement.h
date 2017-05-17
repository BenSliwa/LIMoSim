#ifndef LIMOSIM_UIELEMENT_H
#define LIMOSIM_UIELEMENT_H

#include <QtQuick/QQuickPaintedItem>
#include <QColor>
#include <QDebug>
#include <QPainter>
#include <QPolygonF>
#include <QDebug>

#include "graph.h"
#include "ui/uisettings.h"
#include "epscreator.h"

#include "ui/map/viewport.h"

namespace LIMoSim
{

struct Margin
{
    double left;
    double right;
    double top;
    double bottom;
};

struct ExportConfig
{
    QPointF offset;
    double width;
    double height;
};


class UiElement : public QQuickPaintedItem
{
    Q_OBJECT
public:
    UiElement(QQuickItem *_parent = 0);

    void drawLineOnCanvas(const QPointF &_from, const QPointF &_to, const LineStyle &_style);

    void drawLine(const QPointF &_from, const QPointF &_to, const LineStyle &_style);
    void drawText(const QString &_text, const QPointF &_position, const TextStyle &_style);
    void drawBox();

    QPointF getCanvasPoint(const Point &_point);


    void exportEps(const QString &_path);
    void exportPng(const QString &_path);

    static QStringList readFile(const QString &_path);
    static void writeToFile(const QString &_data, const QString &_path);

    double map(double _value, double _inMin, double _inMax, double _outMin, double _outMax);
    double map(double _value, double _from, double _to);
    QPointF mapToEpsCanvas(const QPointF &_point, double _width, double _height, const QPointF &_offset = QPointF());
    QPointF mapToEpsCanvas(const QPointF &_point, const ExportConfig &_export);

    QRectF getCenteredRect(const QPointF &_center, double _width, double _height);

protected:
    void paint(QPainter *_painter);
    virtual void handlePaintEvent() = 0;



protected:
    ViewPort *p_viewPort;
    QQuickItem *p_parent;
    QPainter *p_painter;

    UiSettings *p_settings;
    EpsCreator m_eps;

};

}


#endif // UIELEMENT_H
