#include <QtCharts>

#include <ranges>

struct Range
{
    qreal
        min,
        max;
};

struct ComplianceColour
{
    qreal value;
    int colour;
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
        , const std::vector<ComplianceColour>& complianceColourCodings
    )
        : _title(title)
        , _legend(legend())
    {
        _legend->hide();
        createDefaultAxes();
        setTitle(title);

        auto line = new QLineSeries;
        std::vector<QScatterSeries*> pointScatters;
        pointScatters.reserve(points.size());

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
        for(auto colour : complianceColourCodings)
        {
            gradient->setColorAt(colour.value, colour.colour);
        }

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
        if(value < 2)
        {
            return 0x00FF00;
        }
        else if(value < 7)
        {
            return 0xFF8000;
        }
        else
        {
            return 0xFF0000;
        }
    }
};