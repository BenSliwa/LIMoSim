#ifndef LIMOSIM_SELECTIONHANDLER_H
#define LIMOSIM_SELECTIONHANDLER_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QTimer>

#include "core/sim/eventhandler.h"
#include "ui/plot/figure.h"

namespace LIMoSim
{

class CarUi;

class SelectionHandler : public QObject, EventHandler
{
    Q_OBJECT
public:
    SelectionHandler(QObject *_parent = 0);
    ~SelectionHandler();
    static SelectionHandler* getInstance();

    void saveSelection(int _key);
    void loadSelection(int _key);


    void selectCar(CarUi *_car);
    void clearSelection();

    // property accessors
    QList<CarUi*>& getSelectedCars();


private:
    void handleEvent(Event *_event);

private slots:
    void onFigureUpdateTimeout();


private:
    Figure *m_figure;
    Figure *m_boxPlot;
    QList<CarUi*> m_selectedCars;

    Event *m_updateTimer;
    double m_updateInterval_s;

};

}


#endif // LIMOSIM_SELECTIONHANDLER_H
