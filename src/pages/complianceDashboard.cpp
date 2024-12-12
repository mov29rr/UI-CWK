#include "complianceDashboard.hpp"

ComplianceDashboardPage::ComplianceDashboardPage(QWidget* parent)
    : Page("Compliance Dashboard"),
      pollutantFilter(new QComboBox()),
      statusFilter(new QComboBox()),
      locationFilter(new QComboBox()),
      detailedTable(new QTableWidget(0, 6)) {
  setupUI();

  connect(detailedTable->verticalScrollBar(), &QScrollBar::valueChanged, this, &ComplianceDashboardPage::onScroll);
}

ComplianceDashboardPage::~ComplianceDashboardPage() {}

void ComplianceDashboardPage::onMount(const QString hash) {
  QSqlQuery query;
  query.prepare("SELECT ID, LABEL, DEFINITION, UNIT FROM determinand;");
  if (query.exec()) {
    pollutantFilter->clear();
    pollutantFilter->addItem("All Pollutants", -1);

    int i = 0;
    while (query.next()) {
      Pollutant pollutant = {query.value("LABEL").toString(), query.value("UNIT").toString(),
                             query.value("DEFINITION").toString(), 3, 6};

      if (i < 4) {
        m_cards[i].title.setText(pollutant.label);
        QString details = QString("Unit: %1\nMin Threshold: %2\nMax Threshold: %3\nInfo: %4")
                              .arg(pollutant.unit)
                              .arg(3)
                              .arg(6)
                              .arg(pollutant.info);
        m_cards[i].detail.setText(details);
      }

      pollutantFilter->addItem(pollutant.label, query.value("ID").toUInt());
      m_pollutants[query.value("ID").toUInt()] = pollutant;

      i++;
    }

    if (i < 4) {
      for (int k = i; k < 4; k++) {
        m_cards[k].title.setText("No Data");
        m_cards[k].detail.setText("No additional information available.");
      }
    }
  } else {
    qDebug() << "Faild to get determinand:" << query.lastError().text();
    return;
  }

  query.prepare("SELECT ID, LABEL FROM site;");
  if (query.exec()) {
    locationFilter->clear();
    locationFilter->addItem("All Pollutants", -1);

    while (query.next()) {
      locationFilter->addItem(query.value("LABEL").toString(), query.value("ID").toUInt());
    }
  } else {
    qDebug() << "Faild to get determinand:" << query.lastError().text();
    return;
  }

  m_offset = 0;
  applyFilters();
}

void ComplianceDashboardPage::setupUI() {
  // Central Widget
  centralWidget = new QWidget(this);
  mainLayout = new QVBoxLayout();

  // Header
  header = new QLabel("Compliance Dashboard");
  header->setAlignment(Qt::AlignCenter);
  header->setStyleSheet("font-size: 18px; font-weight: bold;");
  mainLayout->addWidget(header);

  // Summary Cards
  cardsLayout = new QHBoxLayout();

  for (int i = 0; i < 4; ++i) {  // Only first 4 rows
    summaryCards[i] = new QFrame();
    summaryCards[i]->setFrameShape(QFrame::StyledPanel);
    summaryCards[i]->setStyleSheet("border: 1px solid #d9d9d9; padding: 10px;");
    // summaryCards[i]->setStyleSheet("background-color: #f2f2f2; border: 1px solid #d9d9d9; padding: 10px;");
    summaryCards[i]->setMinimumHeight(200);
    summaryCards[i]->setMinimumWidth(400);

    QVBoxLayout* cardLayout = new QVBoxLayout();

    m_cards[i].title.setText("No Data");
    m_cards[i].detail.setText("No additional information available.");

    m_cards[i].title.setAlignment(Qt::AlignCenter);
    m_cards[i].detail.setAlignment(Qt::AlignLeft);
    cardLayout->addWidget(&m_cards[i].title);
    cardLayout->addWidget(&m_cards[i].detail);

    summaryCards[i]->setLayout(cardLayout);
    cardsLayout->addWidget(summaryCards[i]);
  }
  // m_cards[0].title.setText("Updated Title");

  mainLayout->addLayout(cardsLayout);

  // Filters Section
  filtersLayout = new QHBoxLayout();

  yearFilter = new QComboBox();
  yearFilter->addItem("All Years", -1);
  // yearFilter->setCurrentIndex(5);
  for (int year = 2020; year <= 2024; ++year) {
    yearFilter->addItem(QString::number(year), year);
  }

  // csv::CSVReader reader1("../temp/Locations.csv");
  locationFilter->addItem("All Locations", -1);

  pollutantFilter->addItem("All Pollutants", -1);

  statusFilter->addItem("All Locations", ComplianceStatus::ALL);
  statusFilter->addItem("good", ComplianceStatus::GOOD);
  statusFilter->addItem("medium", ComplianceStatus::MEDIUM);
  statusFilter->addItem("bad", ComplianceStatus::BAD);

  filterButton = new QPushButton("Filter");

  filtersLayout->addWidget(yearFilter);
  filtersLayout->addWidget(locationFilter);
  filtersLayout->addWidget(pollutantFilter);
  filtersLayout->addWidget(statusFilter);
  filtersLayout->addWidget(filterButton);

  mainLayout->addLayout(filtersLayout);

  // Main Content Area
  contentLayout = new QHBoxLayout();

  // Detailed Table
  detailedTable->setHorizontalHeaderLabels({"Location", "Pollutant", "Level", "Unit", "Compliance", "Date"});
  detailedTable->setMinimumSize(600, 300);  // Keep table smaller
  contentLayout->addWidget(detailedTable, 2);

  // Summary of Important Info
  importantInfo = new QTextEdit();
  importantInfo->setPlaceholderText("Summary of important information...");
  importantInfo->setReadOnly(true);
  importantInfo->setMinimumSize(200, 300);
  contentLayout->addWidget(importantInfo, 1);

  mainLayout->addLayout(contentLayout);

  // Footer
  footer = new QLabel("Data provided by UK Environmental Agency.");
  footer->setAlignment(Qt::AlignCenter);
  footer->setStyleSheet("font-size: 12px; color: gray;");
  mainLayout->addWidget(footer);

  // Set Layout
  centralWidget->setLayout(mainLayout);
  content->addWidget(centralWidget);
  resize(1200, 800);  // Adjust window size to fit larger cards

  // Connect filter button to applyFilters
  connect(filterButton, &QPushButton::clicked, this, &ComplianceDashboardPage::applyFilters);
}

ComplianceDashboardPage::ComplianceStatus ComplianceDashboardPage::calculateComplianceStatus(
    const Measurement& sample) {
  if (m_pollutants.find(sample.pollutant_id) != m_pollutants.end()) {
    Pollutant pollutant = m_pollutants[sample.pollutant_id];

    qreal thresholdRange = pollutant.max - pollutant.min;

    if (sample.value >= pollutant.min && sample.value <= pollutant.max) {
      return ComplianceStatus::GOOD;
    } else if (sample.value >= pollutant.min - 0.2 * thresholdRange &&
               sample.value <= pollutant.max + 0.2 * thresholdRange) {
      return ComplianceStatus::MEDIUM;
    } else {
      return ComplianceStatus::BAD;
    }
  }

  return ComplianceStatus::NONE;  // Return "-" if pollutant is not found in the list
}

void ComplianceDashboardPage::populateStats(const QString& bestLocation, const QString& worstLocation,
                                            const QString& bestYear, const QString& worstYear,
                                            const QString& bestPollutant, const QString& worstPollutant,
                                            int totalSamples, int missingDataCount, int compliantSamples,
                                            int mediumSamples, int nonCompliantSamples) {
  // Only show these stats if the selected filter is "All Categories"
  QString selectedLocation = locationFilter->currentText();
  QString selectedYear = yearFilter->currentText();
  QString selectedPollutant = pollutantFilter->currentText();

  // Show location stats only if "All Locations" is selected
  if (selectedLocation == "All Locations") {
    importantInfo->append(QString("<font color='green'>Highest-performing location: %1</font>").arg(bestLocation));
    importantInfo->append(QString("<font color='red'>Lowest-performing location: %1</font>").arg(worstLocation));
  }

  // Show year stats only if "All Years" is selected
  if (selectedYear == "All Years") {
    importantInfo->append(QString("<font color='green'>Highest-performing year: %1</font>").arg(bestYear));
    importantInfo->append(QString("<font color='red'>Lowest-performing year: %1</font>").arg(worstYear));
  }

  // Show pollutant stats only if "All Pollutants" is selected
  if (selectedPollutant == "All Pollutants") {
    importantInfo->append(QString("<font color='green'>Most compliant pollutant: %1</font>").arg(bestPollutant));
    importantInfo->append(QString("<font color='red'>Least compliant pollutant: %1</font>").arg(worstPollutant));
  }

  // Show general sample statistics
  importantInfo->append(QString("<font color='black'>\n\nSample Statistics:</font>"));
  importantInfo->append(QString("<font color='black'>\n- Total number of samples: %1</font>").arg(totalSamples));
  importantInfo->append(QString("<font color='red'>\n- Samples with missing data: %1</font>").arg(missingDataCount));
  importantInfo->append(QString("<font color='green'>\n- Compliant samples: %1</font>").arg(compliantSamples));
  importantInfo->append(
      QString("<font color='orange'>\n- Samples with medium compliance: %1</font>").arg(mediumSamples));
  importantInfo->append(QString("<font color='red'>\n- Non-compliant samples: %1</font>").arg(nonCompliantSamples));
}

void ComplianceDashboardPage::applyFilters() {
  // Clear the important information box before applying new filters
  importantInfo->clear();

  // Retrieve filter criteria
  int year = yearFilter->currentData().toInt();
  int location_id = locationFilter->currentData().toInt();
  int pollutant_id = pollutantFilter->currentData().toInt();

  QString whereString = "";
  QStringList parmas;

  if (year != -1) {
    parmas << "m.date like :year";
  }

  if (location_id != -1) {
    parmas << "m.site_id = :site_id";
  }

  if (pollutant_id != -1) {
    parmas << "m.determinand_id = :pollutant_id";
  }

  if (parmas.size() != 0) {
    whereString = "where";
  }

  QSqlQuery query;
  QString queryString = QString(
                            "SELECT s.label as SITE_NAME, d.id as DETERMINAND_ID, d.label as DETERMINAND_NAME, d.UNIT, "
                            "m.result as VALUE, "
                            "m.DATE, m.IS_COMPLIANCE as "
                            "status FROM measurement as m "
                            "left join site as s on s.id = m.site_id "
                            "left join determinand as d on d.id = m.determinand_id %1 %2 LIMIT :limit OFFSET :offset;")
                            .arg(whereString)
                            .arg(parmas.join(" and "));

  query.prepare(queryString);
  query.bindValue(":limit", ROW_LIMIT);
  query.bindValue(":offset", m_offset);

  if (year != -1) {
    query.bindValue(":year", QString("%1-%").arg(year));
  }

  if (location_id != -1) {
    query.bindValue(":site_id", location_id);
  }

  if (pollutant_id != -1) {
    query.bindValue(":pollutant_id", pollutant_id);
  }

  if (query.exec()) {
    if (m_offset == 0) {
      m_measurements.clear();
    }

    qDebug() << "exec:" << query.lastQuery() << ROW_LIMIT << m_offset;
    while (query.next()) {
      m_measurements.push_back(Measurement{
          query.value("SITE_NAME").toString(),
          query.value("DETERMINAND_ID").toUInt(),
          query.value("DETERMINAND_NAME").toString(),
          query.value("VALUE").toFloat(),
          query.value("DATE").toString(),
          query.value("UNIT").toString(),
          query.value("status").toBool(),
      });
    }

    m_measurements.shrink_to_fit();
  } else {
    qDebug() << "Faild to get measurement:" << query.lastError().text();
    return;
  }

  detailedTable->setRowCount(0);
  // detailedTable->setRowCount(m_measurements.size());

  // Initialize statistics counters
  int totalSamples = 0, missingDataCount = 0, compliantSamples = 0, mediumSamples = 0, nonCompliantSamples = 0;
  QString bestLocation, worstLocation, bestYear, worstYear, bestPollutant, worstPollutant;
  double bestLocationPerformance = -1, worstLocationPerformance = std::numeric_limits<double>::max();
  double bestYearPerformance = -1, worstYearPerformance = std::numeric_limits<double>::max();
  double bestPollutantPerformance = -1, worstPollutantPerformance = std::numeric_limits<double>::max();

  int row = 0;
  for (const Measurement& measurement : m_measurements) {
    int status = statusFilter->currentData().toInt();
    ComplianceStatus complianceStatus = calculateComplianceStatus(measurement);
    if (status != ComplianceStatus::ALL) {
      if (complianceStatus != status) {
        continue;
      }
    }

    if (complianceStatus == ComplianceStatus::GOOD) {
      compliantSamples++;
    } else if (complianceStatus == ComplianceStatus::MEDIUM) {
      mediumSamples++;
    } else if (complianceStatus == ComplianceStatus::BAD) {
      nonCompliantSamples++;
    } else {
      missingDataCount++;
    }

    // Update Location Stats
    if (complianceStatus != ComplianceStatus::NONE) {
      if (measurement.value > bestLocationPerformance) {
        bestLocation = measurement.location_name;
        bestLocationPerformance = measurement.value;
      }
      if (measurement.value < worstLocationPerformance) {
        worstLocation = measurement.location_name;
        worstLocationPerformance = measurement.value;
      }
    }

    // Update Year Stats
    QDateTime dateTime = QDateTime::fromString(measurement.date, Qt::ISODate);
    if (dateTime.isValid()) {
      QString year = QString::number(dateTime.date().year());

      if (complianceStatus != ComplianceStatus::NONE) {
        if (measurement.value > bestYearPerformance) {
          bestYear = year;
          bestYearPerformance = measurement.value;
        }
        if (measurement.value < worstYearPerformance) {
          worstYear = year;
          worstYearPerformance = measurement.value;
        }
      }
    } else {
      qDebug() << "Invalid date-time format!";
    }

    if (m_pollutants.find(measurement.pollutant_id) != m_pollutants.end()) {
      Pollutant pollutant = m_pollutants[measurement.pollutant_id];

      if (complianceStatus != ComplianceStatus::NONE) {
        if (measurement.value > bestPollutantPerformance) {
          bestPollutant = pollutant.label;
          bestPollutantPerformance = measurement.value;
        }
        if (measurement.value < worstPollutantPerformance) {
          worstPollutant = pollutant.label;
          worstPollutantPerformance = measurement.value;
        }
      }
    }

    detailedTable->insertRow(row);
    detailedTable->setItem(row, 0, new QTableWidgetItem(measurement.location_name));
    detailedTable->setItem(row, 1, new QTableWidgetItem(measurement.pollutant_name));
    detailedTable->setItem(row, 2, new QTableWidgetItem(QString::number(measurement.value)));
    detailedTable->setItem(row, 3, new QTableWidgetItem(measurement.unit));
    detailedTable->setItem(row, 4, new QTableWidgetItem(measurement.date));

    auto setRowBackground = [this, &row](QColor rowColor) {
      for (int column = 0; column < detailedTable->columnCount(); ++column) {
        detailedTable->item(row, column)->setBackground(rowColor);
      }
    };

    switch (complianceStatus) {
      case ComplianceStatus::GOOD:
        detailedTable->setItem(row, 5, new QTableWidgetItem("GOOD"));
        setRowBackground(QColor(0, 255, 0));
        break;
      case ComplianceStatus::MEDIUM:
        detailedTable->setItem(row, 5, new QTableWidgetItem("MEDIUM"));
        setRowBackground(QColor(255, 165, 0));
        break;
      case ComplianceStatus::BAD:
        detailedTable->setItem(row, 5, new QTableWidgetItem("BAD"));
        setRowBackground(QColor(255, 0, 0));
        break;
      default:
        detailedTable->setItem(row, 5, new QTableWidgetItem("-"));
        break;
    }

    row++;
  }

  qDebug() << "row size" << m_measurements.size() << row;

  // Call populateStats to display the calculated statistics
  populateStats(bestLocation, worstLocation, bestYear, worstYear, bestPollutant, worstPollutant, totalSamples,
                missingDataCount, compliantSamples, mediumSamples, nonCompliantSamples);
}
