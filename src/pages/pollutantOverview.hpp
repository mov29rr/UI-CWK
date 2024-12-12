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

  const ComplianceLevels _complianceLevels{.veryLow = 2, .low = 3, .high = 6, .veryHigh = 8};

 public:
  /**
   * Constructs the pollutant overview page.
   */
  PollutantOverviewPage();

 private:
  // TODO
  QWidget* overview() override { return new QWidget; }

 private slots:
  void onSearch();
};