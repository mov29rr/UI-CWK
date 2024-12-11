#include "database.hpp"

namespace database {

Database::Database(const QString &path) : database_path(path) {
  // m_db = QSqlDatabase::addDatabase("QSQLITE", QString("Producer-%1").arg(reinterpret_cast<quintptr>(this)));
  connect();
}

Database::~Database() {
  if (m_db.isOpen()) {
    m_db.close();
  }
}

bool Database::isOpen() const { return m_db.isOpen(); }

void Database::initDatabase() {
  {
    QSqlQuery query(m_db);

    query.prepare("SELECT name FROM sqlite_master WHERE type = 'table';");
    if (!query.exec()) {
      qDebug() << "Failed get all table name:" << query.lastError().text();
      assert(query.exec() && "Failed get all table name");
    }

    std::vector<QString> tableName;
    while (query.next()) {
      tableName.push_back(query.value(0).toString());
    }

    for (auto table : tableName) {
      query.prepare(QString("DROP TABLE IF EXISTS \"%1\";").arg(table));
      if (!query.exec()) {
        qDebug() << "Failed to drop all table:" << tableName << query.lastError().text();
        assert(query.exec() && "Failed to drop all table");
      }
    }

    // Step 2: Loop over the tables and drop each one
    while (query.next()) {
      QString tableName = query.value(0).toString();
      // Step 3: Construct and execute the DROP statement for each table
      QSqlQuery dropQuery(m_db);
      dropQuery.prepare(QString("DROP TABLE IF EXISTS \"%1\";").arg(tableName));
      if (!dropQuery.exec()) {
        qDebug() << "Failed to drop all table:" << tableName << dropQuery.lastError().text();
        assert(query.exec() && "Failed to drop all table");
      }
    }
  }
  {
    QSqlQuery query(m_db);
    query.prepare("PRAGMA journal_mode=WAL;");
    if (!query.exec()) {
      QSqlError error = query.lastError();
      std::cerr << "Error executing query: " << error.text().toStdString() << std::endl;
      std::cerr << "SQL State: " << error.nativeErrorCode().toStdString() << std::endl;
      std::cerr << "Error details: " << error.driverText().toStdString() << std::endl;
      assert(false && "Failed to journal_mode");
    }
  }
  {
    QSqlQuery query(m_db);
    query.prepare("PRAGMA busy_timeout = 5000;");
    if (!query.exec()) {
      QSqlError error = query.lastError();
      std::cerr << "Error executing query: " << error.text().toStdString() << std::endl;
      std::cerr << "SQL State: " << error.nativeErrorCode().toStdString() << std::endl;
      std::cerr << "Error details: " << error.driverText().toStdString() << std::endl;
      assert(false && "Failed to busy_timeout");
    }
  }
  {
    QSqlQuery query(m_db);
    query.prepare(
        "CREATE TABLE IF NOT EXISTS site(ID INTEGER PRIMARY KEY, LABEL TEXT, "
        "NORTH INTEGER, EAST INTEGER, NOTATION TEXT);");
    if (!query.exec()) {
      QSqlError error = query.lastError();
      std::cerr << "Error executing query: " << error.text().toStdString() << std::endl;
      std::cerr << "SQL State: " << error.nativeErrorCode().toStdString() << std::endl;
      std::cerr << "Error details: " << error.driverText().toStdString() << std::endl;
      assert(false && "Failed to create site");
    }
  }

  {
    QSqlQuery query(m_db);
    query.prepare(
        "CREATE TABLE IF NOT EXISTS determinand(ID INTEGER PRIMARY KEY, LABEL "
        "TEXT, DEFINITION TEXT, UNIT TEXT);");
    assert(query.exec() && "Failed to create site");
  }

  {
    QSqlQuery query(m_db);
    query.prepare(
        "CREATE TABLE IF NOT EXISTS measurement(ID INTEGER PRIMARY KEY, SITE_ID INTEGER, DETERMINAND_ID INTEGER, "
        "RESULT REAL, MATERIAL_TYPE TEXT, DATE TEXT, PURPOSE_LABEL TEXT, IS_COMPLIANCE INTEGER, "
        "RESULT_QUALIFIER TEXT, CODED_RESULT_INTERPRETATION TEXT);");
    assert(query.exec() && "Failed to create measurement");
  }
}

void Database::reconnect() {
  m_db.close();
  connect();
};

void Database::connect() {
  std::stringstream ss;
  ss << std::this_thread::get_id();

  m_db = QSqlDatabase::addDatabase("QSQLITE", QString::fromStdString(ss.str()));
  m_db.setDatabaseName(database_path);

  if (!m_db.open()) {
    qDebug() << "Error: connection with database fail";
    assert(false && "faield to open database");
  } else {
    qDebug() << "Database: connection ok";
  }
}

}  // namespace database