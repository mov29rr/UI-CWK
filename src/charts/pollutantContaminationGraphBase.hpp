#pragma once

#include "cards/pointInfo.hpp"

#include <QtWidgets>
#include <QtCharts>

#include <limits>
#include <ranges>
#include <memory>

#define mean(a, b) ((a) + (b)) / 2

/**
 * The range structure, stores minimum and maximum values
 *
 * @tparam T the data type for the range.
 */
template <typename T>
struct Range {
  T min, max;
};

/**
 * The base chart.
 */
class PollutantContaminationGraphBase : public QWidget {
 protected:
  QHBoxLayout* _layout;

  QChart* _chart;
  QChartView* _view;

  std::unique_ptr<PointInfoCard> _pointInfo;

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
 
 private:
  void onPointClicked(const QPointF& point);
};