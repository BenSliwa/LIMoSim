#ifndef LIMOSIM_EPSCREATOR_H
#define LIMOSIM_EPSCREATOR_H

#include <QDebug>
#include <QObject>
#include <QColor>
#include <QRectF>
#include <QPainterPath>
#include "graph.h"

// https://www-cdf.fnal.gov/offline/PostScript/BLUEBOOK.PDF

#include "textstyle.h"

namespace LIMOSIM
{

class EpsCreator : public QObject
{
public:
    EpsCreator(QObject *_parent = 0);


    QString drawLine(const QPointF &_from, const QPointF &_to, const LineStyle &_style = LineStyle());
    QString drawRect(const QRectF &_rect, const QPointF &_offset, const LineStyle &_style);
    QString drawRect(const QRectF &_rect, const LineStyle &_style);
    QString drawPolygon(const QPolygonF &_polygon, const LineStyle &_style);
    QString drawPath(const QPainterPath &_path, const LineStyle &_style);
    QString drawText(const QString &_text, const QPointF &_position, const TextStyle &_style);
    QString drawCircle(const QPointF &_center, double _radius, const LineStyle &_style);
    QString draw(const LineStyle &_style);

    QString translate(double _x, double _y);
    QString rotate(double _angle);

    QString newPath();
    QString closePath();
    QString moveTo(double _x, double _y);
    QString lineTo(double _x, double _y);
    QString fill();
    QString stroke();
    QString lineWidth(double _width);
    QString lineStyle(int _style);


    QString serializeColor(const QColor &_color);
    QString getAlignment(int _horizontal, int _vertical);




};


}


#endif // LIMOSIM_EPSCREATOR_H
