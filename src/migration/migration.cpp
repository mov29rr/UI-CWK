#include <QCoreApplication>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <cassert>
#include <string>

#include "../database/database.hpp"
#include "csv.hpp"

using namespace csv;
using namespace database;

void logError(QSqlQuery *query) {
  QSqlError error = query->lastError();
  std::cerr << "Error executing query: " << error.text().toStdString() << std::endl;
  std::cerr << "SQL State: " << error.nativeErrorCode().toStdString() << std::endl;
  std::cerr << "Error details: " << error.driverText().toStdString() << std::endl;
}

// arg csv_file database_file
int main(int argc, char *argv[]) {
  assert(argc >= 3 && "Invalid number of arguments");

  int arg = 0;
  QCoreApplication app(arg, nullptr);
  CSVReader reader(argv[1]);
  Database dbManager(QString::fromStdString(std::string(argv[2])));
  dbManager.initDatabase();

  int i = 0;

  for (CSVRow &row : reader) {
    QSqlQuery siteExistQuery;
    siteExistQuery.prepare("SELECT ID FROM site WHERE LABEL = (:LABEL) LIMIT 1");
    siteExistQuery.bindValue(":LABEL", QString::fromStdString(row["sample.samplingPoint.label"].get<std::string>()));

    int siteId = -1;
    if (siteExistQuery.exec() && siteExistQuery.next()) {
      siteId = siteExistQuery.value(0).toInt();
    } else {
      QSqlQuery query;
      query.prepare(
          "INSERT INTO site (LABEL, NORTH, EAST, NOTATION) VALUES (:label, "
          ":north, :east, :notation)");
      query.bindValue(":label", QString::fromStdString(row["sample.samplingPoint.label"].get<std::string>()));
      query.bindValue(":north", row["sample.samplingPoint.northing"].get<int>());
      query.bindValue(":east", row["sample.samplingPoint.easting"].get<int>());
      query.bindValue(":notation", QString::fromStdString(row["sample.samplingPoint.notation"].get<std::string>()));

      if (!query.exec()) {
        logError(&query);
        assert(false && "failed to insert site");
      }
      siteId = query.lastInsertId().toInt();
    }

    QSqlQuery determinandExistQuery;
    determinandExistQuery.prepare("SELECT ID FROM determinand WHERE ID = (:ID) LIMIT 1");
    determinandExistQuery.bindValue(":ID", row["determinand.notation"].get<int>());

    int determinandId = -1;
    if (determinandExistQuery.exec() && determinandExistQuery.next()) {
      determinandId = determinandExistQuery.value(0).toInt();
    } else {
      QSqlQuery query;
      query.prepare(
          "INSERT INTO determinand (ID, LABEL, DEFINITION, UNIT) VALUES (:id, :label, "
          ":definition, :unit)");

      query.bindValue(":id", row["determinand.notation"].get<int>());
      query.bindValue(":label", QString::fromStdString(row["determinand.label"].get<std::string>()));
      query.bindValue(":definition", QString::fromStdString(row["determinand.definition"].get<std::string>()));
      query.bindValue(":unit", QString::fromStdString(row["determinand.unit.label"].get<std::string>()));

      if (!query.exec()) {
        logError(&query);
        assert(false && "failed to insert determinand");
      }

      determinandId = query.lastInsertId().toInt();
    }

    QSqlQuery query;
    query.prepare(
        "INSERT INTO measurement (DATE, SITE_ID, PURPOSE_LABEL, IS_COMPLIANCE, MATERIAL_TYPE, RESULT, "
        "RESULT_QUALIFIER, DETERMINAND_ID, CODED_RESULT_INTERPRETATION) VALUES "
        "(:DATE, :SITE_ID, :PURPOSE_LABEL, :IS_COMPLIANCE, :MATERIAL_TYPE, :RESULT, :RESULT_QUALIFIER, "
        ":DETERMINAND_ID, :CODED_RESULT_INTERPRETATION)");

    query.bindValue(":DATE", QString::fromStdString(row["sample.sampleDateTime"].get<std::string>()));
    query.bindValue(":SITE_ID", siteId);
    query.bindValue(":PURPOSE_LABEL", QString::fromStdString(row["sample.purpose.label"].get<std::string>()));
    query.bindValue(":IS_COMPLIANCE", row["sample.isComplianceSample"].get<std::string>() == "true");
    query.bindValue(":MATERIAL_TYPE",
                    QString::fromStdString(row["sample.sampledMaterialType.label"].get<std::string>()));
    query.bindValue(":RESULT", row["result"].get<float>());
    query.bindValue(":RESULT_QUALIFIER", QString::fromStdString(row["resultQualifier.notation"].get<std::string>()));
    query.bindValue(":DETERMINAND_ID", determinandId);
    query.bindValue(":CODED_RESULT_INTERPRETATION",
                    QString::fromStdString(row["codedResultInterpretation.interpretation"].get<std::string>()));

    // QString sql = query.executedQuery();
    // std::cout << "SQL Query: " << sql.toStdString() << std::endl;

    if (!query.exec()) {
      logError(&query);
      assert(false && "failed to insert measurement");
    }

    if (i % 1000000 == 0) {
      std::cout << i << std::endl;
    }
    i++;
  }

  return 0;
}
