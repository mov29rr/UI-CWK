#pragma once

#include <unordered_map>

#include "charts/pollutantContaminationGraph.hpp"
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

  std::unique_ptr<PollutantContaminationGraph> _graph;

  QComboBox* _pcbSelector;
  QDateEdit *_startDateSelector, *_endDateSelector;
  QPushButton* _displayButton;

 public:
  /**
   * Constructs the persistent organic pollutants page.
   */
  PersistentOrganicPollutantsPage();

 private:
  void updateGraph();
};