#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <future>
#include <memory>
#include <mutex>
#include <thread>
#include <type_traits>
#include <vector>

#include "task.h"

class ThreadPool {
 public:
  ThreadPool(size_t nthreads) : m_should_run(true) {
    m_worker_threads.reserve(nthreads);
    for(auto i = 0; i < nthreads; i++) {
      m_worker_threads.emplace_back(std::thread(&ThreadPool::run, this));
    }
  }

  ~ThreadPool() {
    m_should_run.store(false);
    for (auto &thread : m_worker_threads) {
      thread.join();
    }
  }

  template <typename Function, typename... Args>
  auto schedule_task(Function&& f, Args&&... args) {
    // TODO allocator for this
    auto task = new SpecializedTask<Function, Args...>(f, args...);
    std::lock_guard lk(m_mutex);
    m_tasks.push_back(task);
    return task->get_future();
  }

 private:
  void run() {
    while(m_should_run.load()) {
      std::unique_lock lk(m_mutex);
      if (m_tasks.empty()) {
        lk.unlock();
        continue;
      }
      auto task = m_tasks.front();
      m_tasks.erase(m_tasks.begin());
      lk.unlock();
      task->invoke();
    }
  };

  std::vector<Task*> m_tasks; // TODO shared_ptr??
  std::vector<std::thread> m_worker_threads;
  std::mutex m_mutex;
  std::atomic<bool> m_should_run;
};

#endif  // THREADPOOL_H_
