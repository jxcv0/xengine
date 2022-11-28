#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <condition_variable>
#include <cstddef>
#include <cstdlib>
#include <mutex>
#include <thread>
#include <vector>

#include <iostream>

#define L1CLS 64 // TODO move this elsewhere
#define MAX_TASKS 8

struct Task {
  virtual ~Task() = default;
  virtual void process() = 0;
};

/**
 * @brief A pool of worker threads.
 *    TODO use pthreads instead stdlibc++ thread has some shinanigainry.
 *
 * 	  1. Separate readonly (after initialization) from rw variables.
 * 	  2. Group rw variables that are used together into a struct so that
 * 	     memory locations are close together.
 * 	  3. Move rw variables that are shared between threads to thier own
 * 	     cacheline using padding.
 * 	  4. Variables shared by multiple threads but independantly should be
 * 	     moved to TLS.
 */
class ThreadPool {
 public:
  /**
   * @brief Construct a thread pool with a fixed number of worker threads.
   *
   * @param nthread The number of worker threads.
   */
  ThreadPool(std::size_t nthreads) : m_should_run(true), m_index(0) {
    m_worker_threads.reserve(nthreads);
    for (auto i = 0; i < nthreads; i++) {
      m_worker_threads.emplace_back(std::thread(&ThreadPool::run, this));
    }
  }

  /**
   * @brief Stop and join all worker threads.
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

  void stop() {
    std::lock_guard lk(m_mutex);
    m_should_run = false;
  }

  /**
   * @brief Add a task to the task buffer.
   *
   * @param t The task to process on a separate thread.
   */
  void schedule_task(Task &t) {
    std::unique_lock lk(m_mutex);
    m_cv.wait(lk, [this]{ return m_index != MAX_TASKS; });
    m_tasks[m_index++] = &t;
    m_cv.notify_one();
  }

 private:
  /**
   * @brief The main work loop.
   */
  void run() {
    for (;;) {
      std::unique_lock lk(m_mutex);
      m_cv.wait(lk,
                [this] { return m_index != 0 || m_should_run == false; });
      if (m_should_run == false) {
        std::cout << "returning\n";
        return;
      }
      auto task = m_tasks[--m_index];
      lk.unlock();
      m_cv.notify_one();
      task->process();
    }
  };

  unsigned int m_index;
  Task* m_tasks[MAX_TASKS]; // 4 cache lines of pointers.
  std::vector<std::thread> m_worker_threads;
  std::mutex m_mutex;
  std::condition_variable m_cv;
  bool m_should_run;
};

#endif  // THREADPOOL_H_
