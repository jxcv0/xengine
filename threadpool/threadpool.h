#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <future>
#include <memory>
#include <mutex>
#include <thread>
#include <type_traits>
#include <vector>

#include "task.h"

/**
 * @brief A pool of worker threads.
 */
class ThreadPool {
 public:
  /**
   * @brief Construct a thread pool with a fixed number of worker threads.
   *
   * @param nthread The number of worker threads.
   */
  ThreadPool(std::size_t nthreads) : m_should_run(true) {
    m_worker_threads.reserve(nthreads);
    for (auto i = 0; i < nthreads; i++) {
      m_worker_threads.emplace_back(std::thread(&ThreadPool::run, this));
    }
  }

  /**
   * @brief Stop join all worker threads.
   */
  ~ThreadPool() {
    m_should_run.store(false);
    for (auto& thread : m_worker_threads) {
      thread.join();
    }
  }

  /**
   * @brief Add a task to the task buffer and get a future that will store the
   *        result;
   *
   * @param f The function that the task will call.
   * @param args The arguments the function will be called with.
   * @return A future that can be waited on for the result of the task.
   */
  template <typename Function, typename... Args>
  auto schedule_task(Function&& f, Args&&... args) {
    // this must be slow
    auto task = new SpecializedTask<Function, Args...>(f, args...);
    std::lock_guard lk(m_mutex);
    m_tasks.push_back(task);
    return task->get_future();
  }

 private:
  /**
   * @brief The main work loop.
   */
  void run() {
    while (m_should_run.load()) {
      std::unique_lock lk(m_mutex);
      if (m_tasks.empty()) {
        continue;
      }
      auto task = m_tasks.front();
      m_tasks.erase(m_tasks.begin());
      lk.unlock();
      task->invoke();
      delete task; // this must be slow
    }
  };

  std::vector<Task*> m_tasks;
  std::vector<std::thread> m_worker_threads;
  std::mutex m_mutex;
  std::atomic<bool> m_should_run;
};

#endif  // THREADPOOL_H_
