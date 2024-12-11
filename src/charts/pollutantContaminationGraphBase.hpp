#pragma once

#include <QtCharts>
#include <limits>
#include <ranges>

#define RED 0xFF0000
#define AMBER 0xFFB000
#define GREEN 0x00FF00
#define GREY 0x808080

#define mean(a, b) ((a) + (b)) / 2

/**
 * The range structure, stores minimum and maximum values
 *
 * @tparam T the data type for the range.
 */
template <typename T>
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
  qreal veryLow,	///< Very low compliance level
	low,        	///< Low compliance level
	high,       	///< High compliance level
	veryHigh;   	///< Very high compliance level
};

struct PollutantContaminationPoint
{
	QDateTime time;
	qreal concentration;
};

/**
 * The base chart.
 */
class PollutantContaminationGraphBase : public QChart
{
protected:
  	QChartView* _view;

  	QDateTimeAxis* _xAxis;
  	QValueAxis* _yAxis;
  	QColorAxis* _colourAxis;

  	QLineSeries* _line;
  	QScatterSeries *_lowPointScatter, *_mediumPointScatter, *_highPointScatter;

  	ComplianceLevels _complianceLevels;
public:
  	/**
  	 * Constructs the graph.
  	 *
  	 * @param title the graph title^ y-axis concentration range.
  	 * @param complianceLevels the compliance levels.
  	 * @param units the concentration units.
  	 * @param points the point data to add to the graph (optional, can add later using
  	 * BaseChart::addPoints).
  	 */
  	PollutantContaminationGraphBase(const QString& title, ComplianceLevels complianceLevels);

  	/**
  	 * Returns a pointer to the chart view.
  	 *
  	 * @return A mutable pointer to the chart view.
  	 */
  	inline QChartView* view() { return _view; }
  	/**
  	 * Returns a pointer to the chart view.
  	 *
  	 * @return A non-mutable pointer to the chart view.
  	 */
  	inline const QChartView* view() const { return _view; }

	/**
	 * Adds an individual point to the graph.
	 * 
	 * @param point the point to add to the graph.
	 */
	virtual void addPoint(const PollutantContaminationPoint& point);
  	/**
  	 * Adds points to the graph.
  	 * 
  	 * @param points the points to add to the graph.
  	 */
  	void addPoints(const std::vector<PollutantContaminationPoint>& points);
protected:
	void updateGradient(Range<qreal> range);
};