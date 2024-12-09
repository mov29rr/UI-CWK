#pragma once

#include <QtCharts>
#include <ranges>

#include "baseChart.hpp"

/**
 * The pollutant contamination graph chart class.
 */
class PollutantContaminationGraph : public BaseChart {
 public:
  /**
   * Constructs the graph.
   *
   * @param title the graph title.
   * @param timeRange the x-axis time range.
   * @param concentrationRange the y-axis concentration range.
   * @param complianceLevels the compliance levels.
   * @param units the concentration units.
   * @param points the point data to add to the graph (optional, can add later using
   * PollutantContaminationGraph::addPoints).
   */
  PollutantContaminationGraph(const QString& title, Range<QDateTime> timeRange, Range<qreal> concentrationRange,
                              ComplianceLevels complianceLevels, const QString& units,
                              const std::vector<Point>& points = {});

  /**
   * Sets the x-axis time range.
   *
   * @param range the x-axis time range to set to.
   */
  void setTimeRange(Range<QDateTime> range);
  /**
   * Sets the y-axis concentration range.
   *
   * @param range the y-axis concentration range to set to.
   */
  void setConcentrationRange(Range<qreal> range);

  /**
   * Adds points to the graph.
   *
   * @param points the points to add to the graph.
   */
  void addPoints(const std::vector<Point>& points) override;
};