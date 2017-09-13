#include "boxplot.h"
#include <cmath>

namespace LIMoSim
{

BoxPlot::BoxPlot(QQuickItem *_parent) :
    Plot(_parent)
{

}

BoxPlot::~BoxPlot()
{
    QMap<QString,Box*>::iterator it;
    for(it=m_data.begin(); it!=m_data.end(); it++)
    {
        Box *box = it.value();
        delete box;
    }
    m_data.clear();
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void BoxPlot::addValue(double _value, const QString &_boxId)
{
    Box *box = 0;
    if(m_data.contains(_boxId))
        box = m_data.value(_boxId);
    else
    {
        box = new Box();
        m_data.insert(_boxId, box);
        m_xTickLabels << _boxId;

        updateTicks(m_data.size()+1, 5);

    }

    box->add(_value);


}

/*************************************
 *          PROPERTY ACCESSORS       *
 ************************************/


/*************************************
 *             UI PAINTING           *
 ************************************/

void BoxPlot::handlePaintEvent()
{
    p_painter->fillRect(this->boundingRect(), QColor("white"));

    if(m_drawGrid)
        drawGrid();

    int i = 0;
    QMap<QString,Box*>::iterator it;
    for(it=m_data.begin(); it!=m_data.end(); it++)
    {
        Box *box = it.value();
        drawBox(box, i);
        i++;
    }

    if(m_drawBox)
        Plot::drawBox();


    // TODO: getOutliers
}

void BoxPlot::drawBox(Box *_box, int _index)
{
    double lowerQuartile = _box->getQuartile(25);
    double upperQuartile = _box->getQuartile(75);
    double iqr = upperQuartile - lowerQuartile;
    double lowerWhisker = _box->getLowerWhisker(lowerQuartile, iqr);
    double upperWhisker = _box->getUpperWhisker(upperQuartile, iqr);
    double median = _box->getMedian();

    double whiskerWidth = 0.3;
    double boxWidth = 0.5;

    double x = (m_xLim.y-m_xLim.x) / (m_data.size()+1) * (_index+1);


    std::vector<double> outliers = _box->getOutliers(lowerWhisker, upperWhisker);
    for(unsigned int i=0; i<outliers.size(); i++)
        drawOutlier(x, outliers.at(i));

    drawLine(getPlotCoordinate(QPointF(x, lowerWhisker)), getPlotCoordinate(QPointF(x, lowerQuartile)), LineStyle("black", 1, false, false, Qt::DashLine));
    drawLine(getPlotCoordinate(QPointF(x, upperWhisker)), getPlotCoordinate(QPointF(x, upperQuartile)), LineStyle("black", 1, false, false, Qt::DashLine));

    drawRectOnPlot(QPointF(x-boxWidth, lowerQuartile), boxWidth*2, iqr, LineStyle("blue"));

    drawLineOnPlot(QPointF(x-whiskerWidth, lowerWhisker), QPointF(x+whiskerWidth, lowerWhisker), LineStyle("black"));
    drawLineOnPlot(QPointF(x-whiskerWidth, upperWhisker), QPointF(x+whiskerWidth, upperWhisker), LineStyle("black"));
    drawLineOnPlot(QPointF(x-boxWidth, median), QPointF(x+boxWidth, median), LineStyle("red"));


    double mean = _box->getMean();
    double w = 3;
    double w2 = w/sqrt(2);
    QPointF meanPoint(getPlotCoordinate(QPointF(x, mean)));

    drawLine(meanPoint+QPointF(-w,0), meanPoint+QPointF(w,0), LineStyle("black"));
    drawLine(meanPoint+QPointF(0,-w), meanPoint+QPointF(0,w), LineStyle("black"));
    drawLine(meanPoint+QPointF(-w2,-w2), meanPoint+QPointF(w2,w2), LineStyle("black"));
    drawLine(meanPoint+QPointF(-w2,w2), meanPoint+QPointF(w2,-w2), LineStyle("black"));


}

void BoxPlot::drawOutlier(double _x, double _y)
{
    double w = 3;
    QPointF outlier(getPlotCoordinate(QPointF(_x, _y)));
    drawLine(outlier+QPointF(-w,0), outlier+QPointF(w,0), LineStyle("red"));
    drawLine(outlier+QPointF(0,-w), outlier+QPointF(0,w), LineStyle("red"));
}

/*************************************
 *              EPS EXPORT           *
 ************************************/

QString BoxPlot::toEps(const ExportConfig &_export)
{
    QString data = "\n";

    data += m_eps.drawRect(QRectF(_export.offset.x(), _export.offset.y(), _export.width, _export.height), LineStyle("white", 1, true, true));

    if(m_drawGrid)
        data += exportGrid(_export);

    int i = 0;
    QMap<QString,Box*>::iterator it;
    for(it=m_data.begin(); it!=m_data.end(); it++)
    {
        Box *box = it.value();
        data += exportBoxplot(box, i, _export);
        i++;
    }

    if(m_drawBox)
        data += exportBox(_export);

    return data;
}

QString BoxPlot::exportBoxplot(Box *_box, int _index, const ExportConfig &_export)
{
    QString data;

    double lowerQuartile = _box->getQuartile(25);
    double upperQuartile = _box->getQuartile(75);
    double iqr = upperQuartile - lowerQuartile;
    double lowerWhisker = _box->getLowerWhisker(lowerQuartile, iqr);
    double upperWhisker = _box->getUpperWhisker(upperQuartile, iqr);
    double median = _box->getMedian();

    double whiskerWidth = 0.3;
    double boxWidth = 0.5;
    double w = 3;

    double x = (m_xLim.y-m_xLim.x) / (m_data.size()+1) * (_index+1);


    // outlier
    std::vector<double> outliers = _box->getOutliers(lowerWhisker, upperWhisker);
    for(unsigned int i=0; i<outliers.size(); i++)
    {
        LineStyle style("red");
        QPointF outlier = getEpsCanvasCoordinate(x, outliers.at(i), _export);
        data += m_eps.drawLine(outlier+QPointF(-w,0), outlier+QPointF(w,0), style);
        data += m_eps.drawLine(outlier+QPointF(0,-w), outlier+QPointF(0,w), style);
    }

    //
    LineStyle style("black", 1, false, false, Qt::DashLine);
    data += exportPlotLine(x, lowerWhisker, x, lowerQuartile, style, _export);
    data += exportPlotLine(x, upperWhisker, x, upperQuartile, style, _export);
    data += exportPlotLine(x-whiskerWidth, lowerWhisker, x+whiskerWidth, lowerWhisker, LineStyle(), _export);
    data += exportPlotLine(x-whiskerWidth, upperWhisker, x+whiskerWidth, upperWhisker, LineStyle(), _export);
    data += exportPlotLine(x-boxWidth, median, x+boxWidth, median, LineStyle("red"), _export);

    // rect   
    QRectF rect;
    rect.setBottomLeft(getEpsCanvasCoordinate(x-boxWidth, lowerQuartile, _export));
    rect.setTopRight(getEpsCanvasCoordinate(x-boxWidth+boxWidth*2, lowerQuartile+iqr, _export));
    data += m_eps.drawRect(rect, LineStyle("blue"));

    // mean
    double w2 = w/sqrt(2);
    style = LineStyle();
    QPointF meanPoint = getEpsCanvasCoordinate(x, _box->getMean(), _export);
    data += m_eps.drawLine(meanPoint+QPointF(-w,0), meanPoint+QPointF(w,0), style);
    data += m_eps.drawLine(meanPoint+QPointF(0,-w), meanPoint+QPointF(0,w), style);
    data += m_eps.drawLine(meanPoint+QPointF(-w2,-w2), meanPoint+QPointF(w2,w2), style);
    data += m_eps.drawLine(meanPoint+QPointF(-w2,w2), meanPoint+QPointF(w2,-w2), style);

    return data;
}

}
