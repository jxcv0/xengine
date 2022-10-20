#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <future>
#include <memory>
#include <thread>
#include <type_traits>
#include <vector>

#include "task.h"

class ThreadPool {
 public:
  ThreadPool() {
  }

  ~ThreadPool() {}

  template <typename Function, typename... Args>
  auto schedule_task(Function&& f, Args&&... args) {
    auto task = new SpecializedTask<Function, Args...>(f, args...);
    m_tasks.push_back(task);
    return task->get_future();
  }

 private:
  std::vector<Task*> m_tasks;
  std::vector<std::thread> m_worker_threads;
};

#endif  // THREADPOOL_H_
