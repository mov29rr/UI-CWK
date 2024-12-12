#pragma once

#include <QtCharts>
#include <ranges>

#include "pollutantContaminationGraphBase.hpp"

/**
 * The auto scaling pollutant contamination graph chart class.
 */
class AutoScalingPollutantContaminationGraph : public PollutantContaminationGraphBase {
 private:
  float m_top_padding = 0;
  void updateAxis(const PollutantContaminationPoint& point);

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
   * AutoScalingPollutantContaminationGraph::addPoints).
   */
  AutoScalingPollutantContaminationGraph(const QString& title, ComplianceLevels complianceLevels,
                          const std::vector<PollutantContaminationPoint>& points = {});

  void setUnit(const QString& units);

  void clear();

  void setYTitle(const QString& title) { _yAxis->setTitleText(title); }

  void addPoint(const PollutantContaminationPoint& point) override;
};