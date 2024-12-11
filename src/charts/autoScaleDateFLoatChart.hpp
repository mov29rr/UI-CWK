#pragma once

#include <QtCharts>
#include <ranges>

#include "baseChart.hpp"

/**
 * The pollutant contamination graph chart class.
 */
class AutoScaleDateFLoatChart : public BaseChart {
 private:
  float m_top_padding = 0;
  void updateAxis(const Point* point);

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
   * AutoScaleDateFLoatChart::addPoints).
   */
  AutoScaleDateFLoatChart(const QString& title, ComplianceLevels complianceLevels,
                          const std::vector<Point>& points = {});

  void setUnit(const QString& units);

  void clear();

  void addPoint(const Point& point);

  void setYTitle(const QString& title) { _yAxis->setTitleText(title); }

  /**
   * Adds points to the graph.
   *
   * @param points the points to add to the graph.
   */
  void addPoints(const std::vector<Point>& points) override;
};