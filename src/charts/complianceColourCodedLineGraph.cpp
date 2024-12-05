#include "complianceColourCodedLineGraph.hpp"

qreal mean(qreal a, qreal b) { return (a + b) / 2; }

ComplianceColourCodedLineGraph::ComplianceColourCodedLineGraph
    ( const QString& title
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

    const qreal range = yRange.max - yRange.min;

    _xAxis->setLabelFormat("%i");
    addAxis(_xAxis, Qt::AlignBottom);

    _yAxis->setLabelFormat("%i");
    addAxis(_yAxis, Qt::AlignLeft);

    addPoints(points);

    auto gradient = new QLinearGradient(QPointF(0, 0), QPointF(0, 1));
    gradient->setColorAt(_complianceLevels.veryHigh / range, RED);
    gradient->setColorAt(mean(_complianceLevels.low, _complianceLevels.high) / range, AMBER);
    gradient->setColorAt(_complianceLevels.veryLow / range, GREEN);

    _colourAxis->setGradient(*gradient);
    _colourAxis->setLabelsVisible(false);
    _colourAxis->setTickCount(2);
    addAxis(_colourAxis, Qt::AlignLeft);

    setAxesRange(xRange, yRange);

    addSeries(_line);
    _line->attachAxis(_xAxis);
    _line->attachAxis(_yAxis);

    addSeries(_lowPointScatter);
    _lowPointScatter->setColor(GREEN);
    _lowPointScatter->attachAxis(_xAxis);
    _lowPointScatter->attachAxis(_yAxis);
    addSeries(_mediumPointScatter);
    _mediumPointScatter->setColor(AMBER);
    _mediumPointScatter->attachAxis(_xAxis);
    _mediumPointScatter->attachAxis(_yAxis);
    addSeries(_highPointScatter);
    _highPointScatter->setColor(RED);
    _highPointScatter->attachAxis(_xAxis);
    _highPointScatter->attachAxis(_yAxis);

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