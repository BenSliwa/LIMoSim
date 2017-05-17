#include "epscreator.h"
#include <QTextStream>

namespace LIMOSIM
{

EpsCreator::EpsCreator(QObject *_parent) : QObject(_parent)
{

}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

QString EpsCreator::drawLine(const QPointF &_from, const QPointF &_to, const LineStyle &_style)
{
    QString data;
    data += moveTo(_from.x(), _from.y());
    data += lineTo(_to.x(), _to.y());
    data += serializeColor(_style.color);
    data += lineWidth(_style.width);
    data += lineStyle(_style.style);
    data += stroke();

    return data;
}

QString EpsCreator::drawRect(const QRectF &_rect, const QPointF &_offset, const LineStyle &_style)
{
    QRectF rect = _rect;
    rect.moveTo(_offset);

    return drawRect(rect, _style);
}

QString EpsCreator::drawRect(const QRectF &_rect, const LineStyle &_style)
{
    QPainterPath path;
    QRectF rect = _rect;
    path.addRect(rect);

    return drawPath(path, _style);
}

QString EpsCreator::drawPolygon(const QPolygonF &_polygon, const LineStyle &_style)
{
    QPainterPath path;
    path.addPolygon(_polygon);

    return drawPath(path, _style);
}

QString EpsCreator::drawPath(const QPainterPath &_path, const LineStyle &_style)
{
    QString object;
    QTextStream stream(&object);

    QPainterPath path = _path;


    stream << "\nn\n";
    for(int i=0; i<path.elementCount(); i++)
    {
        QPainterPath::Element element = path.elementAt(i);

        QString key;
        switch(element.type)
        {
            case QPainterPath::MoveToElement: key="m"; break;
            case QPainterPath::LineToElement: key="l"; break;
        }

        stream << QString::number(element.x) + " " + QString::number(element.y) + " " + key + "\n";
    }

    if(_style.close)
        stream << "h\n"; // close path

    stream << draw(_style);

    return object;
}

QString EpsCreator::drawText(const QString &_text, const QPointF &_position, const TextStyle &_style)
{
    QString data = "\n";
    data += "q\n";
    data += "/" + _style.font + " " + QString::number(_style.fontSize) + " selectfont\n";

    data += QString::number(_position.x()) + " " + QString::number(_position.y()) + " translate\n";
    data += "0 0 m\n";
    data += "(" + _text + ") false charpath flattenpath pathbbox\n";
    data += "4 2 roll pop pop\n";

    data += "0 0 m\n";
    data += QString::number(_style.rotation) + " rotate\n";
    data += getAlignment(_style.horizontalAlignment, _style.verticalAlignment);

    data += "rmoveto\n";
    data += serializeColor(_style.color);
    data += "(" + _text + ") show\n";
    data += "Q\n";

    return data;
}

QString EpsCreator::drawCircle(const QPointF &_center, double _radius, const LineStyle &_style)
{
    QString data;

    data += QString::number(_center.x()) + " " + QString::number(_center.y()) + " ";
    data += QString::number(_radius);
    data += " 0 360 arc closepath\n";
    data += draw(_style);

    return data;
}

QString EpsCreator::draw(const LineStyle &_style)
{
    QString data;
    data += serializeColor(_style.color);
    if(_style.fill)
    {
        data += fill();
    }
    else
    {
        data += lineWidth(_style.width);
        data += lineStyle(_style.style);
        data += stroke();
    }

    return data;
}

QString EpsCreator::translate(double _x, double _y)
{
    QString data = QString::number(_x) + " " + QString::number(_y) + " translate\n";
    return data;
}

QString EpsCreator::rotate(double _angle)
{
    QString data = QString::number(_angle) + " rotate\n";
    return data;
}

QString EpsCreator::newPath()
{
    return "n\n";
}

QString EpsCreator::closePath()
{
    return "h\n";
}

QString EpsCreator::moveTo(double _x, double _y)
{
    QString data = QString::number(_x) + " " + QString::number(_y) + " m\n";
    return data;
}

QString EpsCreator::lineTo(double _x, double _y)
{
    QString data = QString::number(_x) + " " + QString::number(_y) + " l\n";
    return data;
}

QString EpsCreator::fill()
{
    return "f\n";
}

QString EpsCreator::stroke()
{
    return "S\n";
}

QString EpsCreator::lineWidth(double _width)
{
    return QString::number(_width) + " w\n";
}

QString EpsCreator::lineStyle(int _style)
{
    QString data;
    switch(_style)
    {
        case Qt::DashDotDotLine: break;
        case Qt::DashDotLine: break;
        case Qt::DashLine: data = "[4 4] 0 setdash\n"; break;
        case Qt::DotLine: data = "1 setlinecap [0 3] 0  setdash\n"; break;
        case Qt::SolidLine: data = "[] 0 setdash\n"; break;
    }

    return data;
}

QString EpsCreator::serializeColor(const QColor &_color)
{
    QString data;
    QTextStream stream(&data);

    stream << _color.red()/255.0 << " " << _color.green()/255.0 << " " << _color.blue()/255.0 << " " << "rg\n";

    return data;
}

QString EpsCreator::getAlignment(int _horizontal, int _vertical)
{
    QString data;

    switch(_vertical)
    {
        case Qt::AlignVCenter: data += "-.5"; break;
        case Qt::AlignTop: data += "-1"; break;
        case Qt::AlignBottom: data += "0"; break;
    }
    data += " mul exch ";

    switch(_horizontal)
    {
        case Qt::AlignHCenter: data += "-.5"; break;
        case Qt::AlignLeft: data += "0"; break;
        case Qt::AlignRight: data += "-1"; break;
    }
    data += " mul exch\n";


    return data;
}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

/*************************************
 *           PRIVATE METHODS         *
 ************************************/


}
