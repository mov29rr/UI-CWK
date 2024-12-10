#include "persistentOrganicPollutants.hpp"

PersistentOrganicPollutantsPage::PersistentOrganicPollutantsPage()
    : Page("Persistent Organic Pollutants"), _db("../database/database.sqlite") {
  auto records = _db.query(
      "SELECT DISTINCT ID, LABEL, UNIT FROM determinand "
      "WHERE LABEL LIKE \"PCB %\"");

  for (const auto& record : records) {
    _pcbs.emplace_back(record.field("ID").value().toInt(), record.field("LABEL").value().toString(),
                       record.field("UNIT").value().toString());
  }

  // TODO: Select time frame, max points
  auto selectionLayout = new QHBoxLayout;
  selectionLayout->addWidget(new QLabel("Show concentration of"), 0);

  _pcbSelector = new QComboBox;
  for (auto& pcb : _pcbs) {
    _pcbSelector->addItem(pcb.name, QVariant::fromValue(&pcb));
  }
  selectionLayout->addWidget(_pcbSelector);

  _startDateSelector = new QDateEdit, _endDateSelector = new QDateEdit;
  _startDateSelector->setDate(QDate(2024, 1, 1));
  _endDateSelector->setDate(QDate(2025, 1, 1));
  _startDateSelector->setCalendarPopup(true);
  _endDateSelector->setCalendarPopup(true);
  _startDateSelector->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
  _endDateSelector->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

  _displayButton = new QPushButton("Display");

  selectionLayout->addWidget(new QLabel("from"), 0);
  selectionLayout->addWidget(_startDateSelector, 0);
  selectionLayout->addWidget(new QLabel("to"), 0);
  selectionLayout->addWidget(_endDateSelector, 0);
  selectionLayout->addWidget(_displayButton);
  selectionLayout->addStretch();

  layout->addLayout(selectionLayout, 0);

  connect(_displayButton, &QPushButton::clicked, this, &PersistentOrganicPollutantsPage::updateGraph);

  updateGraph();
}

void PersistentOrganicPollutantsPage::onMount(const QString hash) {
  if (!toMount(hash)) {
    return;
  }

  // TODO: move query here
}

void PersistentOrganicPollutantsPage::updateGraph() {
  auto& pcb = *_pcbSelector->currentData().value<PcbDeterminand*>();

  const auto &startDate = _startDateSelector->date().startOfDay(), endDate = _endDateSelector->date().startOfDay();

  auto records = _db.query(
      "SELECT RESULT, DATE FROM measurement "
      "WHERE DETERMINAND_ID = :id "
      "AND DATE BETWEEN :startDate AND :endDate "
      "ORDER BY DATE ASC",
      std::vector<QueryBinding>{{":id", pcb.id},
                                {":startDate", QDateTime(QDate(2024, 1, 1), QTime(0, 0, 0))},
                                {":endDate", QDateTime(QDate(2025, 1, 1), QTime(0, 0, 0))}});

  std::vector<PollutantContaminationGraph::Point> measurements;
  for (const auto& record : records) {
    measurements.emplace_back(record.field("date").value().toDateTime(), record.field("result").value().toReal());
  }

  auto graph = new PollutantContaminationGraph("Persistent Organic Pollutants", {startDate, endDate}, {0, 30},
                                               ComplianceLevels{.veryLow = 2, .low = 3, .high = 6, .veryHigh = 8},
                                               pcb.units, measurements);

  if (!_graph) {
    _graph.reset(graph);
    layout->addWidget(_graph->view());
  } else {
    layout->replaceWidget(_graph->view(), graph->view());
    _graph.reset(graph);
  }
}