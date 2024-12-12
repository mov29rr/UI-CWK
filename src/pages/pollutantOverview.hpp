#pragma once

#include "page.hpp"
#include "core/pollutantContamination.hpp"
#include "charts/autoScalingPollutantContaminationGraph.hpp"

#include <QLineEdit>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTextBrowser>
#include <iostream>

class PollutantSearchHintModel : public QSqlQueryModel {
  Q_OBJECT
 public slots:
  void setFilter(const QString& text);
};

/**
 * The pollutant overview page.
 */
class PollutantOverviewPage : public Page {
  Q_OBJECT
 private:
  QLineEdit* _searchField;
  AutoScalingPollutantContaminationGraph* _chart;
  QWidget* _contentPlaceholder;
  
  qreal _averageConcentration = 0.0;

  const ComplianceLevels _complianceLevels{.veryLow = 2, .low = 3, .high = 6, .veryHigh = 8};

 public:
  /**
   * Constructs the pollutant overview page.
   */
  PollutantOverviewPage();

 private:
  QWidget* overview() override;

 private slots:
  void onSearch();
};