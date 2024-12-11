#ifndef UTILS_H
#define UTILS_H

#include <QSqlError>
#include <QSqlQuery>
#include <QThread>
#include <cassert>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <string>

#include "../csv.hpp"
#include "database.hpp"
#include "sync.hpp"

namespace database {
void logError(QSqlQuery* query);
void threadWrite(QSqlQuery* query, Sync* mtx, database::Database* m_db = nullptr);
void threadExec(QSqlQuery* query, QString* queryString, Sync* sync, database::Database* m_db);
int threadRead(QSqlQuery* query, Sync* mtx);

std::string parseString(const csv::CSVRow* row, const char* colName);
std::string parseString(csv::CSVField* field);

int parseInt(const csv::CSVRow* row, const char* colName);
int parseInt(csv::CSVField* field);

float parseFloat(const csv::CSVRow* row, const char* colName);
float parseFloat(csv::CSVField* field);

}  // namespace database

#endif  // UTILS_H
