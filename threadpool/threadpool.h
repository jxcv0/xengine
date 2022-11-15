#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <condition_variable>
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
    std::unique_lock lk(m_mutex);
    m_should_run = false;
    lk.unlock();
    for (auto& thread : m_worker_threads) {
      m_cv.notify_all();
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
    // TODO needs allocator
    auto task = new SpecializedTask<Function, Args...>(f, args...);
    std::lock_guard lk(m_mutex);
    m_tasks.push_back(TaskView(task));
    m_cv.notify_one();
    return task->get_future();
  }

 private:
  /**
   * @brief The main work loop.
   */
  void run() {
    for (;;) {
      std::unique_lock lk(m_mutex);
      m_cv.wait(lk,
                [this] { return !m_tasks.empty() || m_should_run == false; });
      if (m_should_run == false) {
        return;
      }
      auto task = m_tasks.front();
      m_tasks.erase(m_tasks.begin());
      lk.unlock();
      m_cv.notify_one();
      task.invoke();
      // delete task;  // TODO see above
    }
  };

  std::vector<TaskView> m_tasks;
  std::vector<std::thread> m_worker_threads;
  std::mutex m_mutex;
  std::condition_variable m_cv;
  bool m_should_run;
};

#endif  // THREADPOOL_H_
