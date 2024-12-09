#include "pollutantContaminationGraph.hpp"

PollutantContaminationGraph::PollutantContaminationGraph(const QString& title, Range<QDateTime> timeRange,
                                                         Range<qreal> concentrationRange,
                                                         ComplianceLevels complianceLevels, const QString& units,
                                                         const std::vector<Point>& points)
    : BaseChart(title, complianceLevels) {
  _yAxis->setLabelFormat("%.2f" + units);

  updateGradient(concentrationRange.min, concentrationRange.max);

  addPoints(points);

  setTimeRange(timeRange);
  setConcentrationRange(concentrationRange);
}

void PollutantContaminationGraph::setTimeRange(Range<QDateTime> range) { _xAxis->setRange(range.min, range.max); }
void PollutantContaminationGraph::setConcentrationRange(Range<qreal> range) {
  _yAxis->setRange(range.min, range.max);
  _colourAxis->setRange(range.min, range.max);
}

void PollutantContaminationGraph::addPoints(const std::vector<Point>& points) {
  for (auto point : points) {
    qint64 msSinceEpoch = point.time.toMSecsSinceEpoch();

    _line->append(msSinceEpoch, point.concentration);

    if (point.concentration <= _complianceLevels.low) {
      _lowPointScatter->append(msSinceEpoch, point.concentration);
    } else if (_complianceLevels.low <= point.concentration && point.concentration <= _complianceLevels.high) {
      _mediumPointScatter->append(msSinceEpoch, point.concentration);
    } else if (point.concentration >= _complianceLevels.high) {
      _highPointScatter->append(msSinceEpoch, point.concentration);
    }
  }
}