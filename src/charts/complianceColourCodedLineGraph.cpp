#include "complianceColourCodedLineGraph.hpp"

#define RED 0xFF0000
#define AMBER 0xFFB000
#define GREEN 0x00FF00
#define GREY 0x808080

qreal mean(qreal a, qreal b) { return (a + b) / 2; }

PollutantContaminationGraph::PollutantContaminationGraph
    ( const QString& title
    , Range xRange
    , Range yRange
    , ComplianceLevels complianceLevels
    , const std::vector<Point>& points
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
    _xAxis->setLabelFormat("MM YYYY");
    _xAxis->setTitleText("Time");
    addAxis(_xAxis, Qt::AlignBottom);

    _yAxis->setLabelFormat("%f");
    _yAxis->setTitleText("Concentration");
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

void PollutantContaminationGraph::setXAxisRange(Range range)
{
    _xAxis->setRange(range.min, range.max);
}
void PollutantContaminationGraph::setYAxisRange(Range range)
{
    _yAxis->setRange(range.min, range.max);
    _colourAxis->setRange(range.min, range.max);
}
void PollutantContaminationGraph::setAxesRange(Range xRange, Range yRange)
{
    setXAxisRange(xRange);
    setYAxisRange(yRange);
}

void PollutantContaminationGraph::addPoints(const std::vector<Point>& points)
{
    for(auto point : points)
    {
        _line->append(point.dateTime.toMSecsSinceEpoch(), point.concentration);

        if(point.concentration <= _complianceLevels.low)
        {
            _lowPointScatter->append(point.dateTime.toMSecsSinceEpoch(), point.concentration);
        }
        else if(_complianceLevels.low <= point.concentration && point.concentration <= _complianceLevels.high)
        {
            _mediumPointScatter->append(point.dateTime.toMSecsSinceEpoch(), point.concentration);
        }
        else if(point.concentration >= _complianceLevels.high)
        {
            _highPointScatter->append(point.dateTime.toMSecsSinceEpoch(), point.concentration);
        }
    }
}