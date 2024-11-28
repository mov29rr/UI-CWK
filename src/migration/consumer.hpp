#pragma once

#include <QSqlQuery>
#include <QThread>
#include <memory>
#include <mutex>
#include <string>

#include "../csv.hpp"
#include "../database/database.hpp"
#include "../database/sync.hpp"
#include "../database/utils.hpp"
#include "queue.hpp"

using namespace csv;

class Consumer : public QThread {
  Q_OBJECT

 public:
  Consumer(TSQueue<CSVRow>& queue, std::string database_path, database::Sync& m_sync)
      : m_queue(queue), m_db(nullptr), database_path(database_path), m_sync(m_sync) {
    // std::cout << "new consumer" << std::endl << std::flush;
    std::stringstream ss;
    ss << std::this_thread::get_id();
    m_id = ss.str();
  }
  ~Consumer() override {};

 protected:
  void run() override;

 private:
  void processRow(const std::unique_ptr<CSVRow> row);

  TSQueue<CSVRow>& m_queue;
  database::Database* m_db;
  std::string database_path;
  database::Sync& m_sync;
  std::string m_id;
};