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
    {
        _legend->hide();
        createDefaultAxes();
        setTitle(title);

        auto line = new QLineSeries;

        QScatterSeries
            *lowPointScatter = new QScatterSeries,
            *mediumPointScatter = new QScatterSeries,
            *highPointScatter = new QScatterSeries;

        for(auto point : points)
        {
            *line << point;

            qreal height = point.y();
            if(height <= complianceLevels.low)
            {
                *lowPointScatter << point; 
            }
            else if(complianceLevels.low <= height && height <= complianceLevels.high)
            {
                *mediumPointScatter << point;
            }
            else if(height >= complianceLevels.high)
            {
                *highPointScatter << point;
            }
        }

        const qreal range = yRange.max - yRange.min;

        auto xAxis = new QValueAxis;
        xAxis->setRange(xRange.min, xRange.max);
        xAxis->setLabelFormat("%i");
        addAxis(xAxis, Qt::AlignBottom);

        auto yAxis = new QValueAxis;
        yAxis->setRange(yRange.min, yRange.max);
        yAxis->setLabelFormat("%i");
        addAxis(yAxis, Qt::AlignLeft);

        auto gradient = new QLinearGradient(QPointF(0, 0), QPointF(0, 1));
        gradient->setColorAt(complianceLevels.veryHigh / range, RED);
        gradient->setColorAt(mean(complianceLevels.low, complianceLevels.high) / range, AMBER);
        gradient->setColorAt(complianceLevels.veryLow / range, GREEN);

        auto colourAxis = new QColorAxis;
        colourAxis->setRange(yRange.min, yRange.max);
        colourAxis->setGradient(*gradient);
        colourAxis->setLabelsVisible(false);
        colourAxis->setTickCount(2);
        addAxis(colourAxis, Qt::AlignLeft);

        addSeries(line);
        line->attachAxis(xAxis);
        line->attachAxis(yAxis);

        addSeries(lowPointScatter);
        lowPointScatter->setColor(GREEN);
        lowPointScatter->attachAxis(xAxis);
        lowPointScatter->attachAxis(yAxis);
        addSeries(mediumPointScatter);
        mediumPointScatter->setColor(AMBER);
        mediumPointScatter->attachAxis(xAxis);
        mediumPointScatter->attachAxis(yAxis);
        addSeries(highPointScatter);
        highPointScatter->setColor(RED);
        highPointScatter->attachAxis(xAxis);
        highPointScatter->attachAxis(yAxis);

        _view = new QChartView(this);
        _view->setRenderHint(QPainter::Antialiasing);
    }

    inline QChartView* view() { return _view; }
private:
    qreal mean(qreal a, qreal b)
    {
        return (a + b) / 2;
    }
};