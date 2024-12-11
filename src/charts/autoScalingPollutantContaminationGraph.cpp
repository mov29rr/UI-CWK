#include "autoScalingPollutantContaminationGraph.hpp"

AutoScalingPollutantContaminationGraph::AutoScalingPollutantContaminationGraph(const QString& title, ComplianceLevels complianceLevels,
                                                 const std::vector<PollutantContaminationPoint>& points)
    : PollutantContaminationGraphBase(title, complianceLevels) {
  addPoints(points);
}

void AutoScalingPollutantContaminationGraph::setUnit(const QString& units) { _yAxis->setLabelFormat("%.2f " + units); }

void AutoScalingPollutantContaminationGraph::clear() {
  _line->clear();
  _lowPointScatter->clear();
  _mediumPointScatter->clear();
  _highPointScatter->clear();
  QDateTime date = QDateTime::fromMSecsSinceEpoch(0);
  _xAxis->setRange(date, date);
  _yAxis->setRange(0, 0);
}

void AutoScalingPollutantContaminationGraph::updateAxis(const PollutantContaminationPoint& point) {
  qint64 currentMinX = _xAxis->min().toMSecsSinceEpoch();
  qint64 currentMaxX = _xAxis->max().toMSecsSinceEpoch();

  if (currentMinX == 0) {
    auto epoch = QDateTime::fromMSecsSinceEpoch(point.time.toMSecsSinceEpoch());
    _xAxis->setRange(epoch, epoch);
  } else {
    _xAxis->setRange(QDateTime::fromMSecsSinceEpoch(std::min(currentMinX, point.time.toMSecsSinceEpoch())),
                     QDateTime::fromMSecsSinceEpoch(std::max(currentMaxX, point.time.toMSecsSinceEpoch())));
  }

  qreal currentMinY = _yAxis->min();
  qreal currentMaxY = _yAxis->max() - m_top_padding;

  // add 10% padding to top
  m_top_padding = currentMaxY - currentMinY;

  if (m_top_padding == 0) {
    m_top_padding = currentMaxY;
  }
  m_top_padding *= 0.1;

  const qreal min = std::min(currentMinY, static_cast<qreal>(point.concentration));
  const qreal max = std::max(currentMaxY, static_cast<qreal>(point.concentration));

  _yAxis->setRange(min, max + m_top_padding);

  updateGradient({0, max + m_top_padding});
}

void AutoScalingPollutantContaminationGraph::addPoint(const PollutantContaminationPoint& point)
{
  PollutantContaminationGraphBase::addPoint(point);

  updateAxis(point);
}