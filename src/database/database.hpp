#pragma once

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

namespace database {

namespace SiteCol {
const QString ID = "ID";
const QString LABEL = "LABEL";
const QString NORTH = "NORTH";
const QString EAST = "EAST";
const QString NOTATION = "NOTATION";
};  // namespace SiteCol

namespace DeterminandCol {
const QString ID = "ID";
const QString LABEL = "LABEL";
const QString DEFINITION = "DEFINITION";
const QString UNIT = "UNIT";
};  // namespace DeterminandCol

namespace MeasurementCol {
const QString ID = "ID";
const QString DATE = "DATE";
const QString SITE_ID = "SITE_ID";
const QString PURPOSE_LABEL = "PURPOSE_LABEL";
const QString IS_COMPLIANCE = "IS_COMPLIANCE";
const QString MATERIAL_TYPE = "MATERIAL_TYPE";
const QString RESULT = "RESULT";
const QString RESULT_QUALIFIER = "RESULT_QUALIFIER";
const QString DETERMINAND_ID = "DETERMINAND_ID";
const QString CODED_RESULT_INTERPRETATION = "CODED_RESULT_INTERPRETATION";
};  // namespace MeasurementCol

class Database {
 public:
  /**
   * @brief Constructor
   *
   * Constructor sets up connection with db and opens it
   * @param path - absolute path to db file
   */
  Database(const QString &path);

  /**
   * @brief Destructor
   *
   * Close the db connection
   */
  ~Database();

  bool isOpen() const;

  void initDatabase();

  void reconnect();
  inline QSqlQuery getQuery() { return QSqlQuery(m_db); };
  inline void transaction() { m_db.transaction(); };
  inline void commit() { m_db.commit(); };
  inline void rollback() { m_db.rollback(); };

  inline void close() {
    // std::cout << "close" << std::endl;
    if (m_db.isOpen()) {
      m_db.close();
    }
  };

 private:
  void connect();

  QString database_path;
  QSqlDatabase m_db;
  QSqlQuery m_query;
};

}  // namespace database