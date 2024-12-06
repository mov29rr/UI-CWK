#include "complianceColourCodedLineGraph.hpp"

#define RED 0xFF0000
#define AMBER 0xFFB000
#define GREEN 0x00FF00
#define GREY 0x808080

qreal mean(qreal a, qreal b) { return (a + b) / 2; }

ComplianceColourCodedLineGraph::ComplianceColourCodedLineGraph
    ( const QString& title
    , const QString& xTitle
    , const QString& yTitle
    , Range xRange
    , Range yRange
    , ComplianceLevels complianceLevels
    , const std::vector<QPointF>& points = {}
)
    : _xAxis(new QValueAxis)
    , _yAxis(new QValueAxis)
    , _colourAxis(new QColorAxis)
    , _line(new QLineSeries)
    , _lowPointScatter(new QScatterSeries)
    , _mediumPointScatter(new QScatterSeries)
    , _highPointScatter(new QScatterSeries)
    , _complianceLevels(std::move(complianceLevels))
{
    setTitle(title);

    createDefaultAxes();
    auto legend = this->legend();
    legend->hide();

    // Adding the axes
    _xAxis->setLabelFormat("%i");
    _xAxis->setTitleText(xTitle);
    addAxis(_xAxis, Qt::AlignBottom);

    _yAxis->setLabelFormat("%i");
    _yAxis->setTitleText(yTitle);
    addAxis(_yAxis, Qt::AlignLeft);

    // Adding the points to the series
    addPoints(points);

    // Adding the colour gradient axis
    const qreal range = yRange.max - yRange.min;

    auto gradient = new QLinearGradient(QPointF(0, 0), QPointF(0, 1));
    gradient->setColorAt(_complianceLevels.veryHigh / range, RED);
    gradient->setColorAt(mean(_complianceLevels.low, _complianceLevels.high) / range, AMBER);
    gradient->setColorAt(_complianceLevels.veryLow / range, GREEN);

    _colourAxis->setGradient(*gradient);
    _colourAxis->setLabelsVisible(false);
    _colourAxis->setTickCount(2);
    addAxis(_colourAxis, Qt::AlignRight);

    setAxesRange(xRange, yRange);

    // Adding the compliance regions
    QColor
        lowRegionColour(GREEN),
        midRegionColour(AMBER),
        highRegionColour(RED);
    lowRegionColour.setAlphaF(0.25f);
    midRegionColour.setAlphaF(0.25f);
    highRegionColour.setAlphaF(0.25f);

    QLineSeries
        *bottomLine = new QLineSeries,
        *lowLine = new QLineSeries,
        *highLine = new QLineSeries,
        *topLine = new QLineSeries;
    *bottomLine << QPointF(xRange.min, yRange.min) << QPointF(xRange.max, yRange.min);
    *lowLine << QPointF(xRange.min, _complianceLevels.low) << QPointF(xRange.max, _complianceLevels.low);
    *highLine << QPointF(xRange.min, _complianceLevels.high) << QPointF(xRange.max, _complianceLevels.high);
    *topLine << QPointF(xRange.min, yRange.max) << QPointF(xRange.max, yRange.max);

    QAreaSeries
        *lowArea = new QAreaSeries(bottomLine, lowLine),
        *midArea = new QAreaSeries(lowLine, highLine),
        *highArea = new QAreaSeries(highLine, topLine);

    addSeries(lowArea);
    lowArea->attachAxis(_xAxis);
    lowArea->attachAxis(_yAxis);
    lowArea->setColor(lowRegionColour);
    addSeries(midArea);
    midArea->attachAxis(_xAxis);
    midArea->attachAxis(_yAxis);
    midArea->setColor(midRegionColour);
    addSeries(highArea);
    highArea->attachAxis(_xAxis);
    highArea->attachAxis(_yAxis);
    highArea->setColor(highRegionColour);

    // Adding the line
    addSeries(_line);
    _line->attachAxis(_xAxis);
    _line->attachAxis(_yAxis);
    _line->setColor(Qt::blue);

    // Adding the points
    addSeries(_lowPointScatter);
    _lowPointScatter->attachAxis(_xAxis);
    _lowPointScatter->attachAxis(_yAxis);
    _lowPointScatter->setColor(GREEN);
    addSeries(_mediumPointScatter);
    _mediumPointScatter->attachAxis(_xAxis);
    _mediumPointScatter->attachAxis(_yAxis);
    _mediumPointScatter->setColor(AMBER);
    addSeries(_highPointScatter);
    _highPointScatter->attachAxis(_xAxis);
    _highPointScatter->attachAxis(_yAxis);
    _highPointScatter->setColor(RED);

    _view = new QChartView(this);
    _view->setRenderHint(QPainter::Antialiasing);
}

void ComplianceColourCodedLineGraph::setXAxisRange(Range range)
{
    _xAxis->setRange(range.min, range.max);
}
void ComplianceColourCodedLineGraph::setYAxisRange(Range range)
{
    _yAxis->setRange(range.min, range.max);
    _colourAxis->setRange(range.min, range.max);
}
void ComplianceColourCodedLineGraph::setAxesRange(Range xRange, Range yRange)
{
    setXAxisRange(xRange);
    setYAxisRange(yRange);
}

void ComplianceColourCodedLineGraph::addPoints(const std::vector<QPointF>& points)
{
    for(auto point : points)
    {
        *_line << point;

        qreal height = point.y();
        if(height <= _complianceLevels.low)
        {
            *_lowPointScatter << point; 
        }
        else if(_complianceLevels.low <= height && height <= _complianceLevels.high)
        {
            *_mediumPointScatter << point;
        }
        else if(height >= _complianceLevels.high)
        {
            *_highPointScatter << point;
        }
    }
}