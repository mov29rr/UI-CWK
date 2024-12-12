#include "pollutantOverview.hpp"

#include <QCompleter>
#include <QStringListModel>

#include "database/dbConnection.hpp"
#include "charts/complianceDial.hpp"

void PollutantSearchHintModel::setFilter(const QString& text) {
  QSqlQuery query;
  query.prepare("SELECT LABEL FROM determinand ");

  if (!query.exec()) {
    std::cerr << "Failed to query db" << std::endl;
  }

  setQuery(std::move(query));
}

PollutantOverviewPage::PollutantOverviewPage()
    : Page("Pollutant Overview Page"),
      _searchField(new QLineEdit),
      _contentPlaceholder(new QWidget),
      _chart(new AutoScalingPollutantContaminationGraph("Pollutant", _complianceLevels)) {
  // create searchbar
  auto searchLabel = new QLabel(tr("Search:"));
  searchLabel->setBuddy(_searchField);
  _searchField->setPlaceholderText("Enter a pollutant name");

  auto searchModel = new PollutantSearchHintModel;

  auto completer = new QCompleter(searchModel, this);
  completer->setCaseSensitivity(Qt::CaseInsensitive);

  _searchField->setCompleter(completer);

  connect(_searchField, &QLineEdit::textChanged, searchModel, &PollutantSearchHintModel::setFilter);
  connect(_searchField, &QLineEdit::returnPressed, this, &PollutantOverviewPage::onSearch);

  auto searchLayout = new QHBoxLayout();
  searchLayout->addWidget(searchLabel);
  searchLayout->addWidget(_searchField);

  content->addLayout(searchLayout);
  content->addSpacing(10);

  // divider
  auto divider1 = new QFrame();
  divider1->setFrameShape(QFrame::HLine);
  divider1->setFrameShadow(QFrame::Sunken);

  content->addWidget(divider1);

  // information about pollutant depending on the pollutant chosen
  auto pollutantLabel = new QLabel(tr("Pollutant"));
  pollutantLabel->setAlignment(Qt::AlignLeft);

  content->addWidget(pollutantLabel);
  content->addSpacing(10);
  content->addWidget(_contentPlaceholder);
}

void PollutantOverviewPage::onSearch() {
  const QString label = _searchField->text();

  QSqlQuery getDeterminantQuery;
  getDeterminantQuery.prepare("SELECT ID FROM determinand WHERE LABEL = :label LIMIT 1");
  getDeterminantQuery.bindValue(":label", label);
  if (!getDeterminantQuery.exec() || !getDeterminantQuery.next()) {
    std::cerr << "Failed to find pollutant" << std::endl;
    return;
  }

  int id = getDeterminantQuery.value(0).toInt();

  _chart->clear();

  DbConnection db("../database/database.sql");
  auto records = db.query(
      "SELECT RESULT, DATE FROM measurement "
      "WHERE DETERMINAND_ID = :id "
      "ORDER BY DATE ASC",
      std::vector<QueryBinding>{{":id", id}});

  std::vector<PollutantContaminationPoint> measurements;
  for (const auto& record : records) {
    measurements.emplace_back(record.field("date").value().toDateTime(), record.field("result").value().toReal());
  }

  _averageConcentration = std::accumulate(measurements.cbegin(), measurements.cend(), 0.0,
                                          [](qreal sum, const PollutantContaminationPoint& measurement) {
                                            return sum + measurement.concentration;
                                          }) /
                          measurements.size();

  _chart->addPoints(measurements);

  content->replaceWidget(_contentPlaceholder, _chart);
  _contentPlaceholder = _chart;
}

QWidget* PollutantOverviewPage::overview() {
  return new ComplianceDial(_averageConcentration, {0, 10}, _complianceLevels);
}