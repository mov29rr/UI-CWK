#pragma once

#include <QComboBox>
#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QVBoxLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <random>
#include <string>

// #include "charts/compilanceDateGraph.hpp"
#include "charts/dateFloatChart.hpp"
#include "page.hpp"

struct CompoundType {
  QVariant id;
  QString label;
  QString unit;

  CompoundType(QVariant id, QString label, QString unit) : id(id), label(label), unit(unit) {};
};

/**
 * The pollutant overview page
 */
class FlourinatedCompoundsPage : public Page {
 private:
  std::vector<CompoundType> m_compound;

  QComboBox *m_compound_select = new QComboBox(this);
  QComboBox *m_site_select = new QComboBox(this);

  DateFloatChart *m_chart = nullptr;
  QChartView *m_chart_view = nullptr;

 public:
  /**
   * Constructs the pollutant overview page
   */
  FlourinatedCompoundsPage();

 private slots:
  void onCompoundChange(int index);
  void onSiteChange(int index);
};