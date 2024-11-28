#include "consumer.hpp"

void Consumer::run() {
  if (m_db == nullptr) {
    m_db = new database::Database(QString::fromStdString(database_path));
  }

  int i = 0;

  while (true) {
    // CSVRow row = std::move(m_queue.pop());
    // if (row != nullptr) {
    // std::cout << "row: " << i << std::endl << std::flush;

    processRow(std::make_unique<CSVRow>(std::move(m_queue.pop())));
    i++;
    // }
  }
}

void Consumer::processRow(const std::unique_ptr<CSVRow> row) {
  // std::cout << "processOrw" << std::endl << std::flush;

  QSqlQuery query = m_db->getQuery();
  query.prepare("SELECT ID FROM site WHERE LABEL = (:LABEL) LIMIT 1");
  // auto value = (*row)["sample.samplingPoint.label"];
  // std::cout << "site label: " << value.get<std::string>() << std::endl << std::flush;

  // std::cout << "site label: " << (*row)["sample.samplingPoint.label"].get<std::string>() << std::endl << std::flush;

  query.bindValue(":LABEL", QString::fromStdString(database::parseString(row.get(), "sample.samplingPoint.label")));
  // siteExistQuery.bindValue(":LABEL",
  // QString::fromStdString((*row)["sample.samplingPoint.label"].get<std::string>()));

  int siteId = -1;
  m_db->transaction();
  if (database::threadRead(&query, &m_sync) && query.next()) {
    siteId = query.value(0).toInt();
    m_db->rollback();
  } else {
    query.prepare(
        "INSERT INTO site (LABEL, NORTH, EAST, NOTATION) VALUES (:label, "
        ":north, :east, :notation)");

    query.bindValue(":label", QString::fromStdString((*row)["sample.samplingPoint.label"].get<std::string>()));
    query.bindValue(":north", database::parseInt(row.get(), "sample.samplingPoint.easting"));
    query.bindValue(":east", database::parseInt(row.get(), "sample.samplingPoint.easting"));
    query.bindValue(":notation", QString::fromStdString((*row)["sample.samplingPoint.notation"].get<std::string>()));

    database::threadWrite(&query, &m_sync);
    // std::cout << "free lock" << m_id << std::endl << std::flush;

    m_db->commit();
    siteId = query.lastInsertId().toInt();
  }

  // std::cout << "site" << std::endl << std::flush;

  query.prepare("SELECT ID FROM determinand WHERE ID = (:ID) LIMIT 1");
  // std::cout << "id sea " << (*row)["determinand.notation"].get<>() << std::endl << std::flush;

  query.bindValue(":ID", database::parseInt(row.get(), "determinand.notation"));

  int determinandId = -1;

  if (database::threadRead(&query, &m_sync) && query.next()) {
    determinandId = query.value(0).toInt();
  } else {
    query.prepare(
        "INSERT INTO determinand (ID, LABEL, DEFINITION, UNIT) VALUES (:id, :label, "
        ":definition, :unit)");

    // std::cout << "id bind" << std::endl << std::flush;

    query.bindValue(":id", database::parseInt(row.get(), "determinand.notation"));
    query.bindValue(":label", QString::fromStdString((*row)["determinand.label"].get<std::string>()));
    query.bindValue(":definition", QString::fromStdString((*row)["determinand.definition"].get<std::string>()));
    query.bindValue(":unit", QString::fromStdString((*row)["determinand.unit.label"].get<std::string>()));

    database::threadWrite(&query, &m_sync);
    // std::cout << "free lock" << m_id << std::endl << std::flush;

    determinandId = query.lastInsertId().toInt();
  }

  // std::cout << "determinand" << std::endl << std::flush;

  query.prepare(
      "INSERT INTO measurement (DATE, SITE_ID, PURPOSE_LABEL, IS_COMPLIANCE, MATERIAL_TYPE, RESULT, "
      "RESULT_QUALIFIER, DETERMINAND_ID, CODED_RESULT_INTERPRETATION) VALUES "
      "(:DATE, :SITE_ID, :PURPOSE_LABEL, :IS_COMPLIANCE, :MATERIAL_TYPE, :RESULT, :RESULT_QUALIFIER, "
      ":DETERMINAND_ID, :CODED_RESULT_INTERPRETATION)");

  query.bindValue(":DATE", QString::fromStdString((*row)["sample.sampleDateTime"].get<std::string>()));
  query.bindValue(":SITE_ID", siteId);
  query.bindValue(":PURPOSE_LABEL", QString::fromStdString((*row)["sample.purpose.label"].get<std::string>()));
  query.bindValue(":IS_COMPLIANCE", (*row)["sample.isComplianceSample"].get<std::string>() == "true" ? 1 : 0);
  query.bindValue(":MATERIAL_TYPE",
                  QString::fromStdString((*row)["sample.sampledMaterialType.label"].get<std::string>()));
  query.bindValue(":RESULT", (*row)["result"].get<float>());
  query.bindValue(":RESULT_QUALIFIER", QString::fromStdString((*row)["resultQualifier.notation"].get<std::string>()));
  query.bindValue(":DETERMINAND_ID", determinandId);
  query.bindValue(":CODED_RESULT_INTERPRETATION",
                  QString::fromStdString((*row)["codedResultInterpretation.interpretation"].get<std::string>()));

  // QString sql = query.executedQuery();
  // std::cout << "SQL Query: " << sql.toStdString() << std::endl;
  // std::cerr << "Error executing query: " << row->to_json() << std::endl;

  database::threadWrite(&query, &m_sync, m_db);
  // std::cout << "free lock" << m_id << std::endl << std::flush;
}