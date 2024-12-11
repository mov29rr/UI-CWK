#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

template <typename T>
class TSQueue {
 private:
  std::queue<T> m_queue;
  std::mutex m_mutex;
  std::condition_variable m_cond;
  int i = 0;
  bool m_done = false;

 public:
  void push(T item) {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_queue.push(item);

    m_cond.notify_one();
  }

  bool pop(T& item) {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cond.wait(lock, [this]() { return !m_queue.empty() || (m_done && m_queue.empty()); });

    if (m_done && m_queue.empty()) {
      return false;
    }

    item = std::move(m_queue.front());
    m_queue.pop();
    // std::cout << "queue: " << m_queue.size() << std::endl << std::flush;
    i++;

    if (i % 1000 == 0) {
      std::cout << i << std::endl;
    }
    return true;
  }

  void done() { m_done = true; }
};