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

struct ComplianceLevels
{
    qreal
        veryLow,
        low,
        high,
        veryHigh;
};

qreal mean(qreal a, qreal b)
{
    return (a + b) / 2;
}

class ComplianceColouredChart : public QChart
{
private:
    QString _title;
    QLegend* _legend;

    QChartView* _view;

    ComplianceLevels _complianceLevels;
public:
    ComplianceColouredChart
        ( const QString& title
        , const std::vector<QPointF>& points
        , Range xRange
        , Range yRange
        , ComplianceLevels complianceLevels
    )
        : _title(title)
        , _legend(legend())
        , _complianceLevels(std::move(complianceLevels))
    {
        _legend->hide();
        createDefaultAxes();
        setTitle(title);

        auto line = new QLineSeries;
        std::vector<QScatterSeries*> pointScatters;
        pointScatters.reserve(points.size());

        const qreal range = yRange.max - yRange.min;

        for(auto point : points)
        {
            auto scatter = new QScatterSeries;
            *line << point;
            *scatter << point;
            pointScatters.push_back(scatter);
        }

        auto xAxis = new QValueAxis;
        xAxis->setRange(xRange.min, xRange.max);
        xAxis->setLabelFormat("%i");
        addAxis(xAxis, Qt::AlignBottom);

        auto yAxis = new QValueAxis;
        yAxis->setRange(yRange.min, yRange.max);
        yAxis->setLabelFormat("%i");
        addAxis(yAxis, Qt::AlignLeft);

        auto gradient = new QLinearGradient(QPointF(0, 0), QPointF(0, 1));
        gradient->setColorAt(_complianceLevels.veryHigh / range, RED);
        gradient->setColorAt(mean(_complianceLevels.low, _complianceLevels.high) / range, AMBER);
        gradient->setColorAt(_complianceLevels.veryLow / range, GREEN);

        auto colourAxis = new QColorAxis;
        colourAxis->setRange(yRange.min, yRange.max);
        colourAxis->setGradient(*gradient);
        colourAxis->setLabelsVisible(false);
        colourAxis->setTickCount(2);
        addAxis(colourAxis, Qt::AlignLeft);

        addSeries(line);
        line->attachAxis(xAxis);
        line->attachAxis(yAxis);

        for(auto scatter : pointScatters)
        {
            addSeries(scatter);
            auto point = scatter->points().first();
            scatter->setColor(mapValueToColour(point.y()));
            scatter->attachAxis(xAxis);
            scatter->attachAxis(yAxis);
        }

        _view = new QChartView(this);
        _view->setRenderHint(QPainter::Antialiasing);
    }

    inline QChartView* view() { return _view; }
private:
    int mapValueToColour(qreal value)
    {
        if(value <= _complianceLevels.low)
        {
            return GREEN;
        }
        else if(_complianceLevels.low <= value && value <= _complianceLevels.high)
        {
            return AMBER;
        }
        else if(value >= _complianceLevels.high)
        {
            return RED;
        }
        else
        {
            return 0;
        }
    }
};