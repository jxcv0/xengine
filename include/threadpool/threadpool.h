#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <future>
#include <memory>
#include <thread>
#include <type_traits>
#include <vector>

#include "i_task.h"
#include "task.h"

class ThreadPool {
 public:
  ThreadPool() {}

  ~ThreadPool() {}

  template <typename Function, typename... Args>
  auto schedule_task(Function&& f, Args&&... args) {
    Task task(f, args...);
    m_tasks.push_back(&task);
    return task.get_future();
  }

 private:
  std::vector<i_Task*> m_tasks;
  std::vector<std::thread> m_worker_threads;
};

#endif  // THREADPOOL_H_
