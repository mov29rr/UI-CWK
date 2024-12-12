#include "flourinatedCompound.hpp"

#include "charts/complianceDial.hpp"

FlourinatedCompoundsPage::FlourinatedCompoundsPage() : Page("Flourinated Compounds Page") {
  QVBoxLayout* filterWrapper = new QVBoxLayout;

  QLabel* compoundLabel = new QLabel("Compound:", this);

  QHBoxLayout* compoundRow = new QHBoxLayout;
  compoundRow->setSpacing(8);
  compoundRow->setAlignment(Qt::AlignLeft);
  compoundRow->addWidget(compoundLabel);
  compoundRow->addWidget(m_compound_select);
  filterWrapper->addLayout(compoundRow);

  QLabel* siteLabel = new QLabel("Site:", this);

  QHBoxLayout* siteRow = new QHBoxLayout;
  siteRow->setSpacing(8);
  siteRow->setAlignment(Qt::AlignLeft);
  siteRow->addWidget(siteLabel);
  siteRow->addWidget(m_site_select);
  filterWrapper->addLayout(siteRow);

  content->addLayout(filterWrapper);

  connect(m_compound_select, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &FlourinatedCompoundsPage::onCompoundChange);

  connect(m_site_select, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &FlourinatedCompoundsPage::onSiteChange);

  m_chart = new AutoScalingPollutantContaminationGraph("florinated compounds", _complianceLevels);

  content->addWidget(m_chart);
}

void FlourinatedCompoundsPage::onMount(const QString hash) {
  int compound_id = -1;

  QSqlQuery query;
  query.prepare("SELECT ID, LABEL, UNIT FROM determinand");
  if (query.exec()) {
    m_compound.clear();
    m_compound_select->clear();

    while (query.next()) {
      if (compound_id == -1) {
        compound_id = query.value("ID").toInt();
      }

      QString compoundName = query.value("LABEL").toString();
      QString determinand_unit = query.value("UNIT").toString();
      m_compound_select->addItem(compoundName, query.value("ID"));
      m_compound.push_back({query.value("ID"), compoundName, determinand_unit});
    }
  } else {
    qDebug() << "Faild to get compound:" << query.lastError().text();
    return;
  }

  onCompoundChange(0);
  onSiteChange(0);
}

void FlourinatedCompoundsPage::onCompoundChange(int index) {
  if (m_compound.size() == 0) {
    return;
  }

  int determinand_id = m_compound_select->itemData(index).toInt();
  CompoundType compound = m_compound.at(index);
  m_chart->setYTitle(QString("%1 (%2)").arg(compound.label).arg(compound.unit));

  QSqlQuery query;
  query.prepare(
      "SELECT DISTINCT s.ID, s.LABEL FROM site as s "
      "left join measurement as m on m.SITE_ID = s.id "
      "where m.DETERMINAND_ID = :determinand;");
  query.bindValue(":determinand", determinand_id);
  if (query.exec()) {
    m_site_select->clear();
    while (query.next()) {
      QString compoundName = query.value("LABEL").toString();
      m_site_select->addItem(compoundName, query.value("ID"));
    }
  } else {
    qDebug() << "Faild to get site:" << query.lastError().text();
  }
}

void FlourinatedCompoundsPage::onSiteChange(int index) {
  m_chart->clear();

  int site_id = m_site_select->itemData(index).toInt();
  int determinand_id = m_compound_select->currentData().toInt();

  QSqlQuery query;
  query.prepare(
      "SELECT RESULT, DATE FROM measurement as d "
      "where d.SITE_ID = :site_id AND d.DETERMINAND_ID = :determinand_id LIMIT 10;");
  query.bindValue(":site_id", site_id);
  query.bindValue(":determinand_id", determinand_id);

  std::vector<PollutantContaminationPoint> measurements;
  measurements.reserve(10);

  if (query.exec()) {
    while (query.next()) {
      QString date = query.value("DATE").toString();
      float value = query.value("RESULT").toFloat();

      QDateTime dateTime = QDateTime::fromString(date, "yyyy-MM-ddThh:mm:ss");

      if (dateTime.isValid()) {
        measurements.emplace_back(dateTime, value);
      } else {
        qDebug() << "Invalid date format:" << date;
      }
    }

    m_chart->addPoints(measurements);

    _averageConcentration = std::accumulate(measurements.cbegin(), measurements.cend(), 0.0,
                                            [](qreal sum, const PollutantContaminationPoint& measurement) {
                                              return sum + measurement.concentration;
                                            }) /
                            measurements.size();

  } else {
    qDebug() << "Faild to get measurement:" << query.lastError().text();
  }
}

QWidget* FlourinatedCompoundsPage::overview() { return new ComplianceDial(_averageConcentration, {0, 10}, _complianceLevels); }