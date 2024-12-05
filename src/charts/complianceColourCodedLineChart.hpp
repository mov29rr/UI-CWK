#include <QtCharts>

#include <ranges>

struct Range
{
    qreal
        min,
        max;
};

#define RED 0xFF0000
#define AMBER 0xFFB000
#define GREEN 0x00FF00
#define GREY 0x808080

struct ComplianceLevels
{
    qreal
        veryLow,
        low,
        high,
        veryHigh;
};

class ComplianceColouredChart : public QChart
{
private:
    QString _title;
    QLegend* _legend;

    QChartView* _view;

    QValueAxis
        *_xAxis,
        *_yAxis;
    QColorAxis* _colourAxis;

    QLineSeries* _line;
    QScatterSeries
        *_lowPointScatter,
        *_mediumPointScatter,
        *_highPointScatter;

    ComplianceLevels _complianceLevels;
public:
    ComplianceColouredChart
        ( const QString& title
        , Range xRange
        , Range yRange
        , ComplianceLevels complianceLevels
        , const std::vector<QPointF>& points = {}
    )
        : _title(title)
        , _legend(legend())
        , _xAxis(new QValueAxis)
        , _yAxis(new QValueAxis)
        , _colourAxis(new QColorAxis)
        , _line(new QLineSeries)
        , _lowPointScatter(new QScatterSeries)
        , _mediumPointScatter(new QScatterSeries)
        , _highPointScatter(new QScatterSeries)
        , _complianceLevels(std::move(complianceLevels))
    {
        _legend->hide();
        createDefaultAxes();
        setTitle(title);

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

    inline QChartView* view() { return _view; }

    void setXAxisRange(Range range)
    {
        _xAxis->setRange(range.min, range.max);
    }
    void setYAxisRange(Range range)
    {
        _yAxis->setRange(range.min, range.max);
        _colourAxis->setRange(range.min, range.max);
    }
    void setAxesRange(Range xRange, Range yRange)
    {
        setXAxisRange(xRange);
        setYAxisRange(yRange);
    }

    void addPoints(const std::vector<QPointF>& points)
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
private:
    inline qreal mean(qreal a, qreal b)
    {
        return (a + b) / 2;
    }
};