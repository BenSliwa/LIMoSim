#include "uielement.h"
#include <QQuickWindow>
#include <QTextStream>
#include <QDir>
#include <QFile>




namespace LIMoSim
{

UiElement::UiElement(QQuickItem *_parent) :
    QQuickPaintedItem(_parent),
    p_viewPort(ViewPort::getInstance()),
    p_parent(_parent),
    p_painter(nullptr),
    p_settings(UiSettings::getInstance())
{

}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/


void UiElement::drawLineOnCanvas(const QPointF &_from, const QPointF &_to, const LineStyle &_style)
{
    p_painter->setPen(_style.toPen());
    p_painter->drawLine(_from, _to);
}

void UiElement::drawLine(const QPointF &_from, const QPointF &_to, const LineStyle &_style)
{
    QPointF from = getCanvasPoint(Point{_from.x(), _from.y()});
    QPointF to = getCanvasPoint(Point{_to.x(), _to.y()});

    drawLineOnCanvas(from, to, _style);
}

void drawTextF(QPainter & painter, qreal x, qreal y, Qt::Alignment flags,
              const QString & text, QRectF * boundingRect = nullptr)
{
   const qreal size = 32767.0;
   QPointF corner(x, y - size);
   if (flags & Qt::AlignHCenter) corner.rx() -= size/2.0;
   else if (flags & Qt::AlignRight) corner.rx() -= size;
   if (flags & Qt::AlignVCenter) corner.ry() += size/2.0;
   else if (flags & Qt::AlignTop) corner.ry() += size;
   else flags |= Qt::AlignBottom;
   QRectF rect(corner, QSizeF(size, size));
   painter.drawText(rect, flags, text, boundingRect);
}

void UiElement::drawText(const QString &_text, const QPointF &_position, const TextStyle &_style)
{
    p_painter->save();
    p_painter->translate(_position);
    p_painter->rotate(_style.rotation);



    QFont font;
    font.setFamily(_style.font);
    font.setPixelSize(_style.fontSize);
    p_painter->setFont(font);
    p_painter->setPen(_style.color);


    drawTextF(*p_painter, 0, 0, (Qt::AlignmentFlag)_style.verticalAlignment | (Qt::AlignmentFlag)_style.horizontalAlignment, _text);

    p_painter->restore();
}


void UiElement::drawBox()
{
    QRectF rect(0, 0, width()-1, height()-1);

    p_painter->setPen("black");
    p_painter->drawRect(rect);
}

QPointF UiElement::getCanvasPoint(const Point &_point) // ui coordinate system
{
    double x = _point.x;
    double y = height() - _point.y;

    return QPointF(x,y);
}

void UiElement::exportEps(const QString &_path)
{

}

void UiElement::exportPng(const QString &_path)
{
    QQuickWindow *window = this->window();
    QImage image = window->grabWindow();

    QRect rect = this->boundingRect().toRect();
    rect.moveTo(position().toPoint());

    image = image.copy(rect);

    QPixmap pix = QPixmap::fromImage(image);
    pix.save(_path);

}

QStringList UiElement::readFile(const QString &_path)
{
    QStringList lines;

    QFile file(_path);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);

        while(!in.atEnd())
        {
            QString line = in.readLine();
            lines << line;
        }

        file.close();
    }
    else
        qDebug() << "UiElement::readFile failed" << _path;



    return lines;
}

void UiElement::writeToFile(const QString &_data, const QString &_path)
{
    QFile file(_path);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        stream << _data;
    }
    else
        qDebug() << "UiElement::writeToFile failed to open" << _path;
}

double UiElement::map(double _value, double _inMin, double _inMax, double _outMin, double _outMax)
{
    return (_value-_inMin) * (_outMax-_outMin) / (_inMax-_inMin) + _outMin;
}

double UiElement::map(double _value, double _from, double _to)
{
    return map(_value, 0, _from, 0, _to);
}

QPointF UiElement::mapToEpsCanvas(const QPointF &_point, double _width, double _height, const QPointF &_offset)
{
    double x = map(_point.x(), width(), _width) + _offset.x();
    double y = map(_point.y(), height(), _height) + _offset.y();

    return QPointF(x, y);
}

QPointF UiElement::mapToEpsCanvas(const QPointF &_point, const ExportConfig &_export)
{
    return mapToEpsCanvas(_point, _export.width, _export.height, _export.offset);
}

QRectF UiElement::getCenteredRect(const QPointF &_center, double _width, double _height)
{
    double x = _center.x() - _width/2;
    double y = _center.y() - height()/2;

    return QRectF(x, y, _width, _height);
}


void UiElement::paint(QPainter *_painter)
{
    p_painter = _painter;
    handlePaintEvent();
}

}
