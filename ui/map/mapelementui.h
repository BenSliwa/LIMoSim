#ifndef LIMOSIM_MAPELEMENTUI_H
#define LIMOSIM_MAPELEMENTUI_H

#include "ui/plot/uielement.h"
#include "mapui.h"

#include "LIMoSim/location/position.h"
#include "ui/uieventmanager.h"
#include "ui/uisettings.h"
#include "ui/editor/roadbuilder.h"
#include "ui/editor/selectionhandler.h"

#include "ui/plot/epscreator.h"

namespace LIMoSim
{
class UiManager;

namespace SELECTION
{
    enum{
        NO_SELECTION,
        SOFT_SELECTION,
        HARD_SELECTION
    };
}

class MapElementUi : public UiElement
{
    Q_OBJECT
public:
    MapElementUi(QQuickItem *_parent = 0);

    double getUiDistance(double _distance_m);
    double getMapDistance(double _pixels);

    QPointF getUiPosition(const Position &_position);
    Position getMapPosition(const QPointF &_point);

    void centerAt(const QPointF &_position);

    void drawLine(const Position &_from, const Position &_to, const LineStyle &_style = LineStyle());



    // for eps export
    QPointF getCanvasPoint(const Vector3d &_position);




    void softSelection();
    void softDeselection();

    //
    void setSelection(int _type);
    int getSelection();

private slots:
    void onRepaintRequest();

protected:
    MapUi *p_mapUi;
    UiEventManager *p_eventManager;
    UiManager *p_uiManager;
    UiSettings *p_settings;

    RoadBuilder *p_editor;

    int m_selection;
};

}

#endif // LIMOSIM_MAPELEMENTUI_H
