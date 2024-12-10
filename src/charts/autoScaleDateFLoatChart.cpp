#include "autoScaleDateFLoatChart.hpp"

AutoScaleDateFLoatChart::AutoScaleDateFLoatChart(const QString& title, ComplianceLevels complianceLevels,
                                                 const std::vector<Point>& points)
    : BaseChart(title, complianceLevels) {
  addPoints(points);
}

void AutoScaleDateFLoatChart::setUnit(const QString& units) { _yAxis->setLabelFormat("%.2f " + units); }

void AutoScaleDateFLoatChart::clear() {
  _line->clear();
  _lowPointScatter->clear();
  _mediumPointScatter->clear();
  _highPointScatter->clear();
  QDateTime date = QDateTime::fromMSecsSinceEpoch(0);
  _xAxis->setRange(date, date);
  _yAxis->setRange(0, 0);
}

void AutoScaleDateFLoatChart::addPoint(const Point& point) {
  qint64 msSinceEpoch = point.time.toMSecsSinceEpoch();

  _line->append(msSinceEpoch, point.concentration);

  if (point.concentration <= _complianceLevels.low) {
    _lowPointScatter->append(msSinceEpoch, point.concentration);
  } else if (_complianceLevels.low <= point.concentration && point.concentration <= _complianceLevels.high) {
    _mediumPointScatter->append(msSinceEpoch, point.concentration);
  } else if (point.concentration >= _complianceLevels.high) {
    _highPointScatter->append(msSinceEpoch, point.concentration);
  }

  updateAxis(&point);
}

void AutoScaleDateFLoatChart::addPoints(const std::vector<Point>& points) {
  for (auto point : points) {
    addPoint(point);
  }
}

void AutoScaleDateFLoatChart::updateAxis(const Point* point) {
  qint64 currentMinX = _xAxis->min().toMSecsSinceEpoch();
  qint64 currentMaxX = _xAxis->max().toMSecsSinceEpoch();

  if (currentMinX == 0) {
    auto epoch = QDateTime::fromMSecsSinceEpoch(point->time.toMSecsSinceEpoch());
    _xAxis->setRange(epoch, epoch);
  } else {
    _xAxis->setRange(QDateTime::fromMSecsSinceEpoch(std::min(currentMinX, point->time.toMSecsSinceEpoch())),
                     QDateTime::fromMSecsSinceEpoch(std::max(currentMaxX, point->time.toMSecsSinceEpoch())));
  }

  qreal currentMinY = _yAxis->min();
  qreal currentMaxY = _yAxis->max() - m_top_padding;

  // add 10% padding to top
  m_top_padding = currentMaxY - currentMinY;

  if (m_top_padding == 0) {
    m_top_padding = currentMaxY;
  }
  m_top_padding *= 0.1;

  const qreal min = std::min(currentMinY, static_cast<qreal>(point->concentration));
  const qreal max = std::max(currentMaxY, static_cast<qreal>(point->concentration));

  _yAxis->setRange(min, max + m_top_padding);

  qDebug() << "update gradiet:" << min << max << m_top_padding;

  updateGradient({ 0, max + m_top_padding });
}