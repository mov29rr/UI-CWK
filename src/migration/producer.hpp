#pragma once

#include <QSqlQuery>
#include <QThread>
#include <memory>

#include "../csv.hpp"
#include "../database/utils.hpp"
#include "queue.hpp"

using namespace csv;

class Producer : public QThread {
  Q_OBJECT

 public:
  Producer(TSQueue<CSVRow> &queue, std::string filename, std::atomic<bool> &done)
      : m_queue(queue), filename(filename) {};

  ~Producer() override {};

 protected:
  void run() override {
    csv::CSVReader reader(filename);

    for (CSVRow &row : reader) {
      m_queue.push(std::move(row));
    }

    m_queue.done();
  };

 private:
  TSQueue<CSVRow> &m_queue;
  std::string filename;
};