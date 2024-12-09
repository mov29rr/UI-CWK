#include "dateFloatChart.hpp"

DateFloatChart::DateFloatChart(QGraphicsItem* parent, Qt::WindowFlags wFlags)
    : QChart(parent, wFlags), series(new QLineSeries(this)) {
  this->setTitle("Date vs Int Chart");
  this->addSeries(series);

  axisX = new QDateTimeAxis(this);
  axisX->setFormat("yyyy-MM-dd");
  axisX->setTitleText("Date");
  // QDateTime date = QDateTime::currentDateTime();
  // axisX->setRange(date.addDays(-1), date);
  this->addAxis(axisX, Qt::AlignBottom);
  series->attachAxis(axisX);

  axisY = new QValueAxis(this);
  axisY->setTitleText("Value");
  // axisY->setRange(0, 40);
  this->addAxis(axisY, Qt::AlignLeft);
  series->attachAxis(axisY);
}

void DateFloatChart::addPoint(const QDateTime& date, float value) {
  qDebug() << "Adding point:" << date.toString("yyyy-MM-dd") << value << series->isVisible();

  series->append(date.toMSecsSinceEpoch(), value);
  adjustAxisRange(date, value);
}

void DateFloatChart::addPoints(const std::vector<QPointF>& points) {
  for (const auto& point : points) {
    series->append(point);
    adjustAxisRange(QDateTime::fromMSecsSinceEpoch(point.x()), point.y());
  }
  this->update();
}

void DateFloatChart::clear() {
  series->clear();
  QDateTime date = QDateTime::fromMSecsSinceEpoch(0);
  axisX->setRange(date, date);
  axisY->setRange(0, 0);
}

void DateFloatChart::adjustAxisRange(const QDateTime& date, float value) {
  qint64 currentMinX = axisX->min().toMSecsSinceEpoch();
  qint64 currentMaxX = axisX->max().toMSecsSinceEpoch();

  qDebug() << "Adding point:" << date.toString("yyyy-MM-dd") << value << series->isVisible();

  if (currentMinX == 0) {
    auto epoch = QDateTime::fromMSecsSinceEpoch(date.toMSecsSinceEpoch());
    axisX->setRange(epoch, epoch);
  } else {
    axisX->setRange(QDateTime::fromMSecsSinceEpoch(std::min(currentMinX, date.toMSecsSinceEpoch())),
                    QDateTime::fromMSecsSinceEpoch(std::max(currentMaxX, date.toMSecsSinceEpoch())));
  }

  qreal currentMinY = axisY->min();
  qreal currentMaxY = axisY->max() - m_top_padding;

  m_top_padding = currentMaxY - currentMinY;

  qDebug() << "top 1 padding:" << m_top_padding;

  if (m_top_padding == 0) {
    m_top_padding = currentMaxY;
  }

  m_top_padding *= 0.1;

  qDebug() << "top padding:" << m_top_padding;

  axisY->setRange(std::min(currentMinY, static_cast<qreal>(value)),
                  std::max(currentMaxY, static_cast<qreal>(value)) + m_top_padding);
}