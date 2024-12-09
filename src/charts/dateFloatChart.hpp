#pragma once

#include <QDateTime>
#include <QPointF>
#include <QtCharts/QChart>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtTypes>
#include <algorithm>
#include <vector>

class DateFloatChart : public QChart {
  Q_OBJECT

 public:
  explicit DateFloatChart(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = Qt::WindowFlags());

  void addPoint(const QDateTime &date, float value);

  void addPoints(const std::vector<QPointF> &points);

  inline void clearData() { series->clear(); }

  void clear();

  void setYTitle(const QString &title) { axisY->setTitleText(title); }

 private:
  QLineSeries *series;
  QDateTimeAxis *axisX;
  QValueAxis *axisY;

  float m_top_padding = 0;

  void adjustAxisRange(const QDateTime &date, float value);
};