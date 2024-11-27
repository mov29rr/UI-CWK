#include <QCoreApplication>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <cassert>
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

// arg csv_file database_file
int main(int argc, char *argv[]) {
  assert(argc >= 3 && "Invalid number of arguments");

  // int *o = nullptr;
  // int u = o[0];

  int arg = 0;
  QCoreApplication app(arg, nullptr);
  Database dbManager(QString::fromStdString(std::string(argv[2])));
  dbManager.initDatabase();
  dbManager.close();
  std::atomic<bool> done(false);
  std::mutex db_mtx;

  database::Sync sync;

  TSQueue<CSVRow> queue;

  // const int num_threads = 4;
  // std::vector<std::thread> consumers;
  // for (int i = 0; i < num_threads; ++i) {
  //   consumers.push_back(std::thread(consumer, std::ref(queue)));
  // }

  try {
    Producer producer(queue, argv[1], done);
    producer.start();

    const int num_threads = 1;
    QList<Consumer *> consumers;
    for (int i = 0; i < num_threads; ++i) {
      Consumer *consumer = new Consumer(queue, std::string(argv[2]), std::ref(sync));
      consumers.append(consumer);
      consumer->start();
    }

    producer.wait();

    for (Consumer *consumer : consumers) {
      consumer->wait();
      delete consumer;
    }
  } catch (int err) {
    std::cout << err << std::endl << std::flush;
  }

  // std::thread producer_thread(producer, std::ref(argv[1]), std::ref(queue), std::ref(done));

  // producer_thread.join();

  // for (auto &t : consumers) {
  //   t.join();
  // }

  // int i = 0;
  // for (CSVRow &row : reader) {
  //   if (i % 1000000 == 0) {
  //     std::cout << i << std::endl;
  //   }
  //   i++;
  // }

  return 0;
}
