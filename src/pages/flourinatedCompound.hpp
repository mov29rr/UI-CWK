#pragma once

#include <QComboBox>
#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QVBoxLayout>

#include "charts/autoScalingPollutantContaminationGraph.hpp"
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

  AutoScalingPollutantContaminationGraph *m_chart = nullptr;
  QChartView *m_chart_view = nullptr;

  const ComplianceLevels _complianceLevels {.veryLow = 2, .low = 3, .high = 6, .veryHigh = 8};
  qreal _averageConcentration = 0.0;

 public:
  /**
   * Constructs the pollutant overview page
   */
  FlourinatedCompoundsPage();

  void onMount(const QString hash) override;

 private:
  QWidget* overview() override;

 private slots:
  void onCompoundChange(int index);
  void onSiteChange(int index);
};