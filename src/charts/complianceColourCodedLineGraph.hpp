#pragma once

#include <QtCharts>

#include <ranges>

/**
 * The range structure, stores minimum and maximum values
 * 
 * @tparam T the value for the range
 */
template<typename T>
struct Range
{
    T
        min,
        max;
};

/**
 * The compliance levels structure
 * 
 * The <low> and <high> regions segment the middle region, from the <low> and <high> regions.
 * The <veryLow> value determines the strongest green value on the colour gradient.
 * The <veryHigh> value determines the strongest red value on the colour gradient.
 * While the strongest amber colour value is marked at the midpoint between the low and high value.
 */
struct ComplianceLevels
{
    qreal
        veryLow,    ///< Very low compliance level 
        low,        ///< Low compliance level
        high,       ///< High compliance level
        veryHigh;   ///< Very high compliance level
};

/**
 * The compliance colour coded line graph chart class
 */
class PollutantContaminationGraph : public QChart
{
public:
    struct Point
    {
        QDateTime dateTime;
        qreal concentration;
    };
private:
    QChartView* _view;

    QDateTimeAxis* _xAxis;
    QValueAxis* _yAxis;
    QColorAxis* _colourAxis;

    QLineSeries* _line;
    QScatterSeries
        *_lowPointScatter,
        *_mediumPointScatter,
        *_highPointScatter;

    ComplianceLevels _complianceLevels;
public:
    /**
     * Constructs the graph
     * 
     * @param title the graph title.
     * @param xRange the x-axis time range.
     * @param yRange the y-axis concentration range.
     * @param complianceLevels the compliance levels.
     * @param points the point data to add to the graph (optional, can add later using PollutantContaminationGraph::addPoints).
     */
    PollutantContaminationGraph
        ( const QString& title
        , Range<QDateTime> xRange
        , Range<qreal> yRange
        , ComplianceLevels complianceLevels
        , const std::vector<Point>& points = {}
    );

    /**
     * Returns a pointer to the chart view
     * 
     * @return A mutable pointer to the chart view.
     */
    inline QChartView* view() { return _view; }
    /**
     * Returns a pointer to the chart view
     * 
     * @return A non-mutable pointer to the chart view.
     */
    inline const QChartView* view() const { return _view; }

    /**
     * Sets the x-axis range.
     * 
     * @param range the x-axis range to set to.
     */
    void setXAxisRange(Range<QDateTime> range);
    /**
     * Sets the y-axis range.
     * 
     * @param range the y-axis range to set to.
     */
    void setYAxisRange(Range<qreal> range);

    /**
     * Adds points to the graph
     * 
     * @param points the points to add to the graph.
     */
    void addPoints(const std::vector<Point>& points);
};