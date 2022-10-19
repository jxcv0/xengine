#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <memory>
#include <type_traits>
#include <vector>
#include <future>

#include "i_task.h"

class ThreadPool {
 public:
  ThreadPool() {
  }

  ~ThreadPool() {
  }

  template <typename Function, typename... Args>
  std::future<typename std::result_of<typename std::decay<Function>::type>(
      typename std::decay<Args>::type...)> schedule_task();

 private:
  std::vector<i_Task*> m_tasks;
};

#endif // THREADPOOL_H_
