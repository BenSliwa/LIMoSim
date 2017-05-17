#include "imagebutton.h"

namespace LIMoSim
{

ImageButton::ImageButton(QQuickItem *_parent) :
    UiElement(_parent),
    m_state(BUTTON_STATE::NEUTRAL),
    m_isHovered(false)
{
    double h = p_parent->height();
    setHeight(h);
    setWidth(h);

    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void ImageButton::mousePressEvent(QMouseEvent *_event)
{
    if(m_state==BUTTON_STATE::ON)
        m_state = BUTTON_STATE::OFF;
    else if(m_state==BUTTON_STATE::OFF)
        m_state = BUTTON_STATE::ON;

    update();

    emit stateChanged(m_state);
}

/*************************************
 *             UI PAINTING           *
 ************************************/

void ImageButton::handlePaintEvent()
{
    QString color;

    if(m_state==BUTTON_STATE::ON)
    {
        color = "darkgreen";
        if(m_isHovered)
            color = "green";
    }
    else if(m_state==BUTTON_STATE::OFF)
    {
        color = "darkred";
        if(m_isHovered)
            color = "red";
    }
    else if(m_state==BUTTON_STATE::NEUTRAL)
    {
        color = "#1c1c1c";
        if(m_isHovered)
            color = "darkgray";
    }

    p_painter->fillRect(this->boundingRect(), color);
    UiElement::drawBox();

    drawText(m_text, QPointF(width()/2, height()/2), TextStyle("Arial", 10, "white"));
}

void ImageButton::hoverEnterEvent(QHoverEvent *_event)
{
    m_isHovered = true;
    update();
}

void ImageButton::hoverLeaveEvent(QHoverEvent *_event)
{
    m_isHovered = false;
    update();
}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/

void ImageButton::setId(const QString &_id)
{
    m_id = _id;
}

void ImageButton::setText(const QString &_text)
{
    m_text = _text;
}

void ImageButton::setState(int _state)
{
    m_state = _state;
}

QString ImageButton::getId()
{
    return m_id;
}

}
