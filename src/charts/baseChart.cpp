#include "baseChart.hpp"

BaseChart::BaseChart(const QString &title, ComplianceLevels complianceLevels)
    : _xAxis(new QDateTimeAxis),
      _yAxis(new QValueAxis),
      _colourAxis(new QColorAxis),
      _line(new QLineSeries),
      _lowPointScatter(new QScatterSeries),
      _mediumPointScatter(new QScatterSeries),
      _highPointScatter(new QScatterSeries),
      _complianceLevels(std::move(complianceLevels)) {
  setTitle(title);

  createDefaultAxes();
  auto legend = this->legend();
  legend->hide();

  // Adding the axes
  _xAxis->setTitleText("Time");
  addAxis(_xAxis, Qt::AlignBottom);

  _yAxis->setTitleText("Concentration");
  addAxis(_yAxis, Qt::AlignLeft);

  _colourAxis->setLabelsVisible(false);
  _colourAxis->setTickCount(2);
  addAxis(_colourAxis, Qt::AlignRight);

  // Adding the compliance regions
  QColor lowRegionColour(GREEN), midRegionColour(AMBER), highRegionColour(RED);
  lowRegionColour.setAlphaF(0.25f);
  midRegionColour.setAlphaF(0.25f);
  highRegionColour.setAlphaF(0.25f);

  auto bottomLine = new QLineSeries, lowLine = new QLineSeries, highLine = new QLineSeries, topLine = new QLineSeries;
  *bottomLine << QPointF(0, 0) << QPointF(1, 0);
  *lowLine << QPointF(0, _complianceLevels.low) << QPointF(1, _complianceLevels.low);
  *highLine << QPointF(0, _complianceLevels.high) << QPointF(1, _complianceLevels.high);
  *topLine << QPointF(0, 1e6) << QPointF(1, 1e6);

  auto lowArea = new QAreaSeries(bottomLine, lowLine), midArea = new QAreaSeries(lowLine, highLine),
       highArea = new QAreaSeries(highLine, topLine);

  addSeries(lowArea);
  lowArea->attachAxis(_yAxis);
  lowArea->setColor(lowRegionColour);
  addSeries(midArea);
  midArea->attachAxis(_yAxis);
  midArea->setColor(midRegionColour);
  addSeries(highArea);
  highArea->attachAxis(_yAxis);
  highArea->setColor(highRegionColour);

  // Adding the line
  addSeries(_line);
  _line->attachAxis(_xAxis);
  _line->attachAxis(_yAxis);
  _line->setColor(Qt::blue);

  // Adding the points
  addSeries(_lowPointScatter);
  _lowPointScatter->attachAxis(_xAxis);
  _lowPointScatter->attachAxis(_yAxis);
  _lowPointScatter->setColor(GREEN);
  addSeries(_mediumPointScatter);
  _mediumPointScatter->attachAxis(_xAxis);
  _mediumPointScatter->attachAxis(_yAxis);
  _mediumPointScatter->setColor(AMBER);
  addSeries(_highPointScatter);
  _highPointScatter->attachAxis(_xAxis);
  _highPointScatter->attachAxis(_yAxis);
  _highPointScatter->setColor(RED);

  _view = new QChartView(this);
  _view->setRenderHint(QPainter::Antialiasing);
}

void BaseChart::updateGradient(Range<qreal> range) {
  const qreal delta = range.max - range.min;

  auto gradient = new QLinearGradient(QPointF(0, 0), QPointF(0, 1));
  gradient->setColorAt(qBound(0.0, _complianceLevels.veryHigh / delta, 1.0), RED);
  gradient->setColorAt(qBound(0.0, mean(_complianceLevels.low, _complianceLevels.high) / delta, 1.0), AMBER);
  gradient->setColorAt(qBound(0.0, _complianceLevels.veryLow / delta, 1.0), GREEN);

  _colourAxis->setGradient(*gradient);
}