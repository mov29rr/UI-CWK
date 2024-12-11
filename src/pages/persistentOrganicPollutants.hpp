#pragma once

#include <unordered_map>

#include "charts/staticScalingPollutantContaminationGraph.hpp"
#include "charts/complianceDial.hpp"
#include "database/dbConnection.hpp"
#include "page.hpp"

/**
 * The PCB determinand structure.
 *
 * Identifies a PCB in the data.
 */
struct PcbDeterminand {
  /// The PCB ID
  int id;
  /// The PCB name
  QString name;
  /// The PCB concentration units
  QString units;
};

/**
 * The persistent organic pollutants page class.
 */
class PersistentOrganicPollutantsPage : public Page {
 private:
  DbConnection _db;
  std::vector<PcbDeterminand> _pcbs;

  std::unique_ptr<StaticScalingPollutantContaminationGraph> _graph;
  qreal _averageConcentration = 0.0;

  QComboBox* _pcbSelector;
  QDateEdit *_startDateSelector, *_endDateSelector;
  QPushButton* _displayButton;

  const ComplianceLevels _complianceLevels {.veryLow = 2, .low = 3, .high = 6, .veryHigh = 8};
  const Range<qreal> _concentrationRange { 0, 20 };
 public:
  /**
   * Constructs the persistent organic pollutants page.
   */
  PersistentOrganicPollutantsPage();

  void onMount(const QString hash) override;

 private:
  void updateGraph();

  QWidget* overview() override;
};