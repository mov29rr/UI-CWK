#include "flourinatedCompound.hpp"

FlourinatedCompoundsPage::FlourinatedCompoundsPage() : Page("Flourinated Compounds Page") {
  QLabel* compoundLabel = new QLabel("Compound:", this);

  layout->addWidget(compoundLabel);
  layout->addWidget(m_compound_select);

  QLabel* siteLabel = new QLabel("Site:", this);

  layout->addWidget(siteLabel);
  layout->addWidget(m_site_select);

  int compound_id = -1;

  QSqlQuery query;
  query.prepare("SELECT ID, LABEL, UNIT FROM determinand");
  if (query.exec()) {
    while (query.next()) {
      if (compound_id == -1) {
        compound_id = query.value("ID").toInt();
        qDebug() << "site name:" << compound_id;
      }

      QString compoundName = query.value("LABEL").toString();
      QString determinand_unit = query.value("UNIT").toString();
      m_compound_select->addItem(compoundName, query.value("ID"));
      m_compound.push_back({query.value("ID"), compoundName, determinand_unit});
    }
  } else {
    qDebug() << "Faild to get compound:" << query.lastError().text();
  }
  connect(m_compound_select, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &FlourinatedCompoundsPage::onCompoundChange);

  connect(m_site_select, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &FlourinatedCompoundsPage::onSiteChange);

  // m_chart->setTitle("Date vs Integer Chart");
  m_chart = new DateFloatChart();

  m_chart_view = new QChartView(m_chart, this);
  m_chart_view->setRenderHint(QPainter::Antialiasing);

  layout->addWidget(m_chart_view);

  onCompoundChange(0);
  onSiteChange(0);

  attachLayout();
}

void FlourinatedCompoundsPage::onCompoundChange(int index) {
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
  qDebug() << "site change:" << index;

  m_chart->clear();

  int site_id = m_site_select->itemData(index).toInt();
  int determinand_id = m_compound_select->currentData().toInt();

  QSqlQuery query;
  query.prepare(
      "SELECT RESULT, DATE FROM measurement as d "
      "where d.SITE_ID = :site_id AND d.DETERMINAND_ID = :determinand_id LIMIT 10;");
  query.bindValue(":site_id", site_id);
  query.bindValue(":determinand_id", determinand_id);
  if (query.exec()) {
    while (query.next()) {
      QString date = query.value("DATE").toString();
      float value = query.value("RESULT").toFloat();

      QDateTime dateTime = QDateTime::fromString(date, "yyyy-MM-ddThh:mm:ss");

      if (dateTime.isValid()) {
        m_chart->addPoint(dateTime, value);
      } else {
        qDebug() << "Invalid date format:" << date;
      }
    }
  } else {
    qDebug() << "Faild to get measurement:" << query.lastError().text();
  }
}
