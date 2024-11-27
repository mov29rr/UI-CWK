#include "database.hpp"

namespace database {

Database::Database(const QString &path) {
  m_db = QSqlDatabase::addDatabase("QSQLITE");
  m_db.setDatabaseName(path);

  if (!m_db.open()) {
    qDebug() << "Error: connection with database fail";
  } else {
    qDebug() << "Database: connection ok";
  }
}

Database::~Database() {
  if (m_db.isOpen()) {
    m_db.close();
  }
}

bool Database::isOpen() const { return m_db.isOpen(); }

void Database::initDatabase() {
  {
    QSqlQuery query;
    query.prepare(
        "CREATE TABLE IF NOT EXISTS site(ID INTEGER PRIMARY KEY, LABEL TEXT, "
        "NORTH INTEGER, EAST INTEGER, NOTATION TEXT);");
    assert(query.exec() && "Failed to create site");
  }

  {
    QSqlQuery query;
    query.prepare(
        "CREATE TABLE IF NOT EXISTS determinand(ID INTEGER PRIMARY KEY, LABEL "
        "TEXT, DEFINITION TEXT, UNIT TEXT);");
    assert(query.exec() && "Failed to create site");
  }

  {
    QSqlQuery query;
    query.prepare(
        "CREATE TABLE IF NOT EXISTS measurement(ID INTEGER PRIMARY KEY, SITE_ID INTEGER, DETERMINAND_ID INTEGER, "
        "RESULT REAL, MATERIAL_TYPE TEXT, DATE TEXT, PURPOSE_LABEL TEXT, IS_COMPLIANCE INTEGER, "
        "RESULT_QUALIFIER TEXT, CODED_RESULT_INTERPRETATION TEXT);");
    assert(query.exec() && "Failed to create measurement");
  }
}

bool Database::createTable() {
  bool success = false;

  QSqlQuery query;
  query.prepare("CREATE TABLE people(id INTEGER PRIMARY KEY, name TEXT);");

  if (!query.exec()) {
    qDebug() << "Couldn't create the table 'people': one might already exist.";
    success = false;
  }

  return success;
}

bool Database::addPerson(const QString &name) {
  bool success = false;

  if (!name.isEmpty()) {
    QSqlQuery queryAdd;
    queryAdd.prepare("INSERT INTO people (name) VALUES (:name)");
    queryAdd.bindValue(":name", name);

    if (queryAdd.exec()) {
      success = true;
    } else {
      qDebug() << "add person failed: " << queryAdd.lastError();
    }
  } else {
    qDebug() << "add person failed: name cannot be empty";
  }

  return success;
}

bool Database::removePerson(const QString &name) {
  bool success = false;

  if (personExists(name)) {
    QSqlQuery queryDelete;
    queryDelete.prepare("DELETE FROM people WHERE name = (:name)");
    queryDelete.bindValue(":name", name);
    success = queryDelete.exec();

    if (!success) {
      qDebug() << "remove person failed: " << queryDelete.lastError();
    }
  } else {
    qDebug() << "remove person failed: person doesnt exist";
  }

  return success;
}

void Database::printAllPersons() const {
  qDebug() << "Persons in db:";
  QSqlQuery query("SELECT * FROM people");
  int idName = query.record().indexOf("name");
  while (query.next()) {
    QString name = query.value(idName).toString();
    qDebug() << "===" << name;
  }
}

bool Database::personExists(const QString &name) const {
  bool exists = false;

  QSqlQuery checkQuery;
  checkQuery.prepare("SELECT name FROM people WHERE name = (:name)");
  checkQuery.bindValue(":name", name);

  if (checkQuery.exec()) {
    if (checkQuery.next()) {
      exists = true;
    }
  } else {
    qDebug() << "person exists failed: " << checkQuery.lastError();
  }

  return exists;
}

bool Database::removeAllPersons() {
  bool success = false;

  QSqlQuery removeQuery;
  removeQuery.prepare("DELETE FROM people");

  if (removeQuery.exec()) {
    success = true;
  } else {
    qDebug() << "remove all persons failed: " << removeQuery.lastError();
  }

  return success;
}

}  // namespace database