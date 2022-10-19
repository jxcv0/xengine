#ifndef TASK_H_
#define TASK_H_

#include <functional>
#include <future>
#include <memory>
#include <type_traits>
#include <utility>

#include "i_task.h"

template <typename Function, typename... Args>
class Task : public i_Task {
  using ReturnType =
      typename std::result_of<typename std::decay<Function>::type(
          typename std::decay<Args>::type...)>::type;

 public:
  Task(Function f, Args... args) {
    mp_task = std::make_shared<std::packaged_task<Function(Args...)>>(
        std::bind(std::forward<Function>(f), std::forward<Args>(args)...));
  }

  /**
   * @brief
   */
  auto get_future() { return mp_task->get_future(); }

  void invoke() override { mp_task(); }

 private:
  std::shared_ptr<std::packaged_task<Function(Args...)>> mp_task;
};

#endif  // TASK_H_
