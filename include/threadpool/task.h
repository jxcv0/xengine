#ifndef TASK_H_
#define TASK_H_

#include <functional>
#include <future>
#include <memory>
#include <type_traits>
#include <utility>

#include "task.h"

class Task {
 public:
  virtual ~Task() {}
  virtual void invoke() = 0;
};

template <typename Function, typename... Args>
class SpecializedTask : public Task {
 public:
  using ReturnType =
      typename std::result_of<typename std::decay<Function>::type(
          typename std::decay<Args>::type...)>::type;

  SpecializedTask(Function f, Args... args) {
    m_task = std::packaged_task<ReturnType()>(
        std::bind(std::forward<Function>(f), std::forward<Args>(args)...));
  }

  /**
   * @brief
   */
  auto get_future() { return m_task.get_future(); }

  void invoke() override { m_task(); }

 private:
  std::packaged_task<ReturnType()> m_task;
};

#endif  // TASK_H_
