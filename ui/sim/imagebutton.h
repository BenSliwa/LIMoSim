#ifndef IMAGEBUTTON_H
#define IMAGEBUTTON_H

#include "ui/plot/uielement.h"


namespace LIMOSIM
{

namespace BUTTON_STATE
{
    enum
    {
        ON,
        OFF,
        NEUTRAL
    };
}

class ImageButton : public UiElement
{
    Q_OBJECT
public:
    ImageButton(QQuickItem *_parent = 0);

    // mouse
    void mousePressEvent(QMouseEvent *_event);
    void hoverEnterEvent(QHoverEvent *_event);
    void hoverLeaveEvent(QHoverEvent *_event);

    // ui painting
    void handlePaintEvent();

    // property accessors
    void setId(const QString &_id);
    void setText(const QString &_text);
    void setState(int _state);

    QString getId();

signals:
    void stateChanged(int _state);

private:
    int m_state;
    bool m_isHovered;

    QString m_id;
    QString m_text;


};

}

#endif // IMAGEBUTTON_H
