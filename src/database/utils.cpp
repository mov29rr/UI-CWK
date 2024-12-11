#include "utils.hpp"

namespace database {
void logError(QSqlQuery* query) {
  QSqlError error = query->lastError();
  std::cerr << "Error executing query: " << error.text().toStdString() << std::endl;
  std::cerr << "Error database error: " << error.databaseText().toStdString() << std::endl;
  std::cerr << "error tyoe: " << error.type() << std::endl;
  std::cerr << "SQL State: " << error.nativeErrorCode().toStdString() << std::endl;
  std::cerr << "Error details: " << error.driverText().toStdString() << std::endl;
}

void threadWrite(QSqlQuery* query, Sync* sync, database::Database* m_db) {
  bool bTransaction = m_db != nullptr;

  std::stringstream ss;
  ss << std::this_thread::get_id();
  std::string m_id = ss.str();
  // std::cout << "wait mutex " << m_id << std::endl << std::flush;

  sync->to_write.store(sync->to_write.load() + 1);

  std::unique_lock<std::shared_mutex> lock(sync->mtx);

  sync->to_write.store(sync->to_write.load() - 1);

  // std::cout << "writting: " << m_id << std::endl << std::flush;

  if (bTransaction) {
    m_db->transaction();
  }

  if (query->exec()) {
    if (bTransaction) {
      m_db->commit();
    }

    sync->cv.notify_all();
    return;
  }

  QSqlError error = query->lastError();
  // std::cerr << "Error executing query: " << error.text().toStdString() << std::endl;
  // std::cerr << "Error database error: " << error.databaseText().toStdString() << std::endl;
  // std::cerr << "error tyoe: " << error.type() << std::endl;
  // std::cerr << "SQL State: " << error.nativeErrorCode().toStdString() << std::endl;
  // std::cerr << "Error details: " << error.driverText().toStdString() << std::endl;

  // std::cout << "reexecute: " << m_id << std::endl << std::flush;

  int i = 1;
  while (error.type() == QSqlError::ErrorType::ConnectionError && error.nativeErrorCode() == "517") {
    QThread::msleep(1000);

    if (query->exec()) {
      if (bTransaction) {
        m_db->commit();
      }

      sync->cv.notify_all();
      return;
    }

    error = query->lastError();

    if (i % 10 == 0) {
      m_db->reconnect();
    }
    i++;
  }

  if (bTransaction) {
    m_db->rollback();
  }

  std::cerr << "Error executing query: " << error.text().toStdString() << std::endl;
  std::cerr << "Error database error: " << error.databaseText().toStdString() << std::endl;
  std::cerr << "error tyoe: " << error.type() << std::endl;
  std::cerr << "SQL State: " << error.nativeErrorCode().toStdString() << std::endl;
  std::cerr << "sql: " << query->executedQuery().toStdString() << std::endl;
  std::cerr << "Error details: " << error.driverText().toStdString() << std::endl;
  assert(false && "failed to call thread wtire");
}

void threadExec(QSqlQuery* query, QString* queryString, Sync* sync, database::Database* m_db) {
  bool bTransaction = m_db != nullptr;

  std::stringstream ss;
  ss << std::this_thread::get_id();
  std::string m_id = ss.str();

  sync->to_write.store(sync->to_write.load() + 1);

  std::unique_lock<std::shared_mutex> lock(sync->mtx);

  sync->to_write.store(sync->to_write.load() - 1);

  if (bTransaction) {
    m_db->transaction();
  }

  if (query->exec(*queryString)) {
    if (bTransaction) {
      m_db->commit();
    }

    sync->cv.notify_all();
    return;
  }

  QSqlError error = query->lastError();

  int i = 1;
  while (error.type() == QSqlError::ErrorType::ConnectionError && error.nativeErrorCode() == "517") {
    QThread::msleep(1000);

    if (query->exec(*queryString)) {
      if (bTransaction) {
        m_db->commit();
      }

      sync->cv.notify_all();
      return;
    }

    error = query->lastError();

    if (i % 10 == 0) {
      m_db->reconnect();
    }
    i++;
  }

  if (bTransaction) {
    m_db->rollback();
  }

  std::cerr << "Error executing query: " << error.text().toStdString() << std::endl;
  std::cerr << "Error database error: " << error.databaseText().toStdString() << std::endl;
  std::cerr << "error tyoe: " << error.type() << std::endl;
  std::cerr << "SQL State: " << error.nativeErrorCode().toStdString() << std::endl;
  std::cerr << "sql: " << query->executedQuery().toStdString() << std::endl;
  std::cerr << "Error details: " << error.driverText().toStdString() << std::endl;
  assert(false && "failed to call thread wtire");
}

int threadRead(QSqlQuery* query, Sync* sync) {
  std::stringstream ss;
  ss << std::this_thread::get_id();
  std::string m_id = ss.str();
  // std::cout << "wait read mutex " << m_id << std::endl << std::flush;

  std::shared_lock<std::shared_mutex> lock(sync->mtx);
  sync->cv.wait(lock, [&]() { return sync->to_write.load() == 0; });

  // std::cout << "reading:  " << m_id << std::endl << std::flush;

  return query->exec();
}

std::string parseString(const csv::CSVRow* row, const char* colName) {
  assert(row != nullptr);

  try {
    return (*row)[std::string(colName)].get<std::string>();
  } catch (...) {
    return "";
  }
}

std::string parseString(csv::CSVField* field) {
  assert(field != nullptr);

  try {
    return field->get<std::string>();
  } catch (...) {
    return "";
  }
}

int parseInt(const csv::CSVRow* row, const char* colName) {
  assert(row != nullptr);

  try {
    csv::CSVField field = (*row)[std::string(colName)];
    if (field.is_int()) {
      return field.get<int>();
    }

    std::cerr << "not int: " << colName << std::endl;

    std::string value = (*row)[std::string(colName)].get<std::string>();
    value.erase(0, value.find_first_not_of(" \t\n\r"));
    value.erase(value.find_last_not_of(" \t\n\r") + 1);
    return std::stoi(value.c_str());
  } catch (std::invalid_argument const& ex) {
    std::string value = (*row)[colName].get<std::string>();
    std::cerr << "failed to parseInt: " << ex.what() << " '" + value + "' " << std::endl;

    std::cout << "std::invalid_argument::what(): " << ex.what() << '\n';
    assert(false && "failed to parseInt");
  } catch (std::out_of_range const& ex) {
    std::string value = (*row)[colName].get<std::string>();
    std::cerr << "failed to parseInt: " << ex.what() << " '" + value + "' " << std::endl;

    std::cout << "std::out_of_range::what(): " << ex.what() << '\n';
    // const long long ll{std::stoll(s, &pos)};
    // std::cout << "std::stoll(" << std::quoted(s) << "): " << ll << "; pos: " << pos << '\n';
    assert(false && "failed to parseInt");
  } catch (const std::exception& e) {
    std::string value = (*row)[colName].get<std::string>();
    std::cerr << "failed to parseInt: " << e.what() << " '" + value + "' " << std::endl;
    assert(false && "failed to parseInt");
  } catch (...) {
    std::string value = (*row)[colName].get<std::string>();
    std::cerr << "failed to parseInt: " << " '" + value + "' " << std::endl;
    assert(false && "failed to parseInt");
  }
}

int parseInt(csv::CSVField* field) {
  assert(field != nullptr);

  try {
    std::string value = field->get<std::string>();
    return std::stoi(value);
  } catch (const std::exception& e) {
    // Catch all standard exceptions derived from std::exception
    std::cerr << "failed to parseInt: " << e.what() << std::endl;
    assert(false && "failed to parseInt");
  }
}

float parseFloat(const csv::CSVRow* row, const char* colName) {
  assert(row != nullptr);

  try {
    std::string value = (*row)[std::string(colName)].get<std::string>();
    return std::stof(value);
  } catch (const std::exception& e) {
    // Catch all standard exceptions derived from std::exception
    std::string value = (*row)[colName].get<std::string>();
    std::cerr << "failed to parseInt: " << e.what() << " '" + value + "' " << std::endl;
    assert(false && "failed to parseInt");
  }
}

float parseFloat(csv::CSVField* field) {
  assert(field != nullptr);

  try {
    std::string value = field->get<std::string>();
    return std::stof(value);
  } catch (const std::exception& e) {
    // Catch all standard exceptions derived from std::exception
    std::cerr << "failed to parseInt: " << e.what() << std::endl;
    assert(false && "failed to parseInt");
  }
}

}  // namespace database
