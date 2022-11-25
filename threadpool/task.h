#ifndef TASK_H_
#define TASK_H_

#include <functional>
#include <future>
#include <memory>
#include <type_traits>
#include <utility>

/**
 * @brief The task interface.
 */
class ITask {
 public:
  virtual ~ITask() {}
  virtual void invoke() = 0;
};

/**
 * @brief Class template that wraps a std::packaged_task. This way the
 *        threadpool can store a task with any arguments and return types.
 *        This solves problems I don't have yet and create problems that I
 *        definitely do have.
 */
template <typename Function, typename... Args>
class Task : public ITask {
 public:
  using ReturnType =
      typename std::result_of<typename std::decay<Function>::type(
          typename std::decay<Args>::type...)>::type;

  /**
   * @brief Construct a SpecializedClass instance.
   *
   * @param f The function the task will execute.
   * @param args The arguments the function will be called with.
   */
  Task(Function f, Args... args) {
    m_task = std::packaged_task<ReturnType()>(
        std::bind(std::forward<Function>(f), std::forward<Args>(args)...));
  }

  /**
   * @brief Desctuctor.
   */
  virtual ~Task() {}

  /**
   * @brief Get the associated future of this task.
   *
   * @return The associated future of this task.
   */
  auto get_future() { return m_task.get_future(); }

  /**
   * @brief Invoke this task.
   */
  void invoke() override { m_task(); }

 private:
  std::packaged_task<ReturnType()> m_task;
};

#endif  // TASK_H_
