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
  Producer(TSQueue<CSVRow> &queue, char *filename, std::atomic<bool> &done)
      : m_queue(queue), filename(filename), done(done) {};

  ~Producer() override {};

 protected:
  void run() override {
    csv::CSVReader reader(filename);

    for (CSVRow &row : reader) {
      // std::cout << "row json: " << row.to_json() << std::endl;
      m_queue.push(std::move(row));
    }

    done.store(true);
  };

 private:
  TSQueue<CSVRow> &m_queue;
  char *filename;
  std::atomic<bool> &done;
};