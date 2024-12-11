#include "pollutantContaminationGraphBase.hpp"

PollutantContaminationGraphBase::PollutantContaminationGraphBase(const QString &title, ComplianceLevels complianceLevels)
    : _chart(new QChart)
    , _xAxis(new QDateTimeAxis),
      _yAxis(new QValueAxis),
      _colourAxis(new QColorAxis),
      _line(new QLineSeries),
      _lowPointScatter(new QScatterSeries),
      _mediumPointScatter(new QScatterSeries),
      _highPointScatter(new QScatterSeries),
      _complianceLevels(std::move(complianceLevels)) {
  auto layout = new QHBoxLayout;
  
  _chart->setTitle(title);

  _chart->createDefaultAxes();
  auto legend = _chart->legend();
  legend->hide();

  // Adding the axes
  _xAxis->setTitleText("Time");
  _chart->addAxis(_xAxis, Qt::AlignBottom);

  _yAxis->setTitleText("Concentration");
  _chart->addAxis(_yAxis, Qt::AlignLeft);

  _colourAxis->setLabelsVisible(false);
  _colourAxis->setTickCount(2);
  _chart->addAxis(_colourAxis, Qt::AlignRight);

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

  _chart->addSeries(lowArea);
  lowArea->attachAxis(_yAxis);
  lowArea->setColor(lowRegionColour);
  _chart->addSeries(midArea);
  midArea->attachAxis(_yAxis);
  midArea->setColor(midRegionColour);
  _chart->addSeries(highArea);
  highArea->attachAxis(_yAxis);
  highArea->setColor(highRegionColour);

  // Adding the line
  _chart->addSeries(_line);
  _line->attachAxis(_xAxis);
  _line->attachAxis(_yAxis);
  _line->setColor(Qt::blue);

  // Adding the points
  _chart->addSeries(_lowPointScatter);
  _lowPointScatter->attachAxis(_xAxis);
  _lowPointScatter->attachAxis(_yAxis);
  _lowPointScatter->setColor(GREEN);
  _chart->addSeries(_mediumPointScatter);
  _mediumPointScatter->attachAxis(_xAxis);
  _mediumPointScatter->attachAxis(_yAxis);
  _mediumPointScatter->setColor(AMBER);
  _chart->addSeries(_highPointScatter);
  _highPointScatter->attachAxis(_xAxis);
  _highPointScatter->attachAxis(_yAxis);
  _highPointScatter->setColor(RED);

  connect(_lowPointScatter, &QScatterSeries::clicked, this, &PollutantContaminationGraphBase::onPointClicked);
  connect(_mediumPointScatter, &QScatterSeries::clicked, this, &PollutantContaminationGraphBase::onPointClicked);
  connect(_highPointScatter, &QScatterSeries::clicked, this, &PollutantContaminationGraphBase::onPointClicked);

  _view = new QChartView(_chart);
  _view->setRenderHint(QPainter::Antialiasing);

  layout->addWidget(_view);

  setLayout(layout);
}

void PollutantContaminationGraphBase::updateGradient(Range<qreal> range) {
  const qreal delta = range.max - range.min;

  auto gradient = new QLinearGradient(QPointF(0, 0), QPointF(0, 1));
  gradient->setColorAt(qBound(0.0, _complianceLevels.veryHigh / delta, 1.0), RED);
  gradient->setColorAt(qBound(0.0, mean(_complianceLevels.low, _complianceLevels.high) / delta, 1.0), AMBER);
  gradient->setColorAt(qBound(0.0, _complianceLevels.veryLow / delta, 1.0), GREEN);

  _colourAxis->setGradient(*gradient);
}

void PollutantContaminationGraphBase::addPoint(const PollutantContaminationPoint& point) {
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

void PollutantContaminationGraphBase::addPoints(const std::vector<PollutantContaminationPoint>& points)
{
  for(const auto& point : points)
  {
    addPoint(point);
  }
}

void PollutantContaminationGraphBase::onPointClicked(const QPointF& point)
{
  PollutantContaminationPoint measurement
  {
    .time = QDateTime::fromMSecsSinceEpoch(point.x()),
    .concentration = point.y()
  };

  // Display
}