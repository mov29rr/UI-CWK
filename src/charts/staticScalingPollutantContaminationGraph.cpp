#include "staticScalingPollutantContaminationGraph.hpp"

StaticScalingPollutantContaminationGraph::StaticScalingPollutantContaminationGraph(
    const QString& title, Range<QDateTime> timeRange, Range<qreal> concentrationRange,
    ComplianceLevels complianceLevels, const QString& units, const std::vector<PollutantContaminationPoint>& points)
    : PollutantContaminationGraphBase(title, complianceLevels) {
  _yAxis->setLabelFormat("%.2f" + units);

  addPoints(points);

  setTimeRange(timeRange);
  setConcentrationRange(concentrationRange);
}

void StaticScalingPollutantContaminationGraph::setTimeRange(Range<QDateTime> range) {
  _xAxis->setRange(range.min, range.max);
}
void StaticScalingPollutantContaminationGraph::setConcentrationRange(Range<qreal> range) {
  _yAxis->setRange(range.min, range.max);
  _colourAxis->setRange(range.min, range.max);

  updateGradient(range);
}
