#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <shared_mutex>

namespace database {
struct Sync {
  std::shared_mutex mtx;
  std::condition_variable_any cv;
  std::atomic<int> to_write{0};
};

}  // namespace database