#pragma once

#include <limits.h>
#include <unistd.h>

#include <QCoreApplication>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <cassert>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "../csv.hpp"
#include "../database/database.hpp"
#include "../database/sync.hpp"
#include "../database/utils.hpp"
#include "consumer.hpp"
#include "producer.hpp"
#include "queue.hpp"

using namespace csv;
using namespace database;

inline int migrate(std::string csvPath, std::string dbPath) {
  int arg = 0;
  QCoreApplication app(arg, nullptr);
  Database dbManager(QString::fromStdString(dbPath));
  dbManager.initDatabase();
  dbManager.close();
  std::atomic<bool> done(false);
  std::mutex db_mtx;

  database::Sync sync;

  TSQueue<CSVRow> queue;

  try {
    Producer producer(queue, csvPath.c_str(), done);
    producer.start();

    const int num_threads = 1;
    QList<Consumer *> consumers;
    for (int i = 0; i < num_threads; ++i) {
      Consumer *consumer = new Consumer(queue, dbPath, std::ref(sync));
      consumers.append(consumer);
      consumer->start();
    }

    producer.wait();

    for (Consumer *consumer : consumers) {
      consumer->wait();
      delete consumer;
    }
  } catch (int err) {
    return 1;
    std::cout << err << std::endl << std::flush;
  }

  return 0;
}