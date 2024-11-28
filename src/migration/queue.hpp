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

 public:
  void push(T item) {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_queue.push(item);

    m_cond.notify_one();
  }

  T pop() {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cond.wait(lock, [this]() { return !m_queue.empty(); });

    T item = std::move(m_queue.front());
    m_queue.pop();
    // std::cout << "queue: " << m_queue.size() << std::endl << std::flush;
    i++;

    if (i % 1000 == 0) {
      std::cout << i << std::endl;
    }
    return item;
  }
};