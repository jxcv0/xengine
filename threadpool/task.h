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
class Task {
 public:
  virtual ~Task() {}
  virtual void invoke() = 0;
};

/**
 * @brief Alternative interface for specializing tasks that so far does not work
 *        for some reason.
 */
// class TaskView {
//  public:
//   template <typename Schedulable>
//   explicit TaskView(const Schedulable& s)
//       : mp_task{&s}
//       , invoke_impl{[](const void *task) {
//           return static_cast<const Schedulable*>(task)->invoke(); }}
//   { }
//
//   void invoke() { invoke_impl(mp_task); }
//  private:
//   const void *mp_task;
//   void (*invoke_impl)(const void*);
// };

/**
 * @brief Class template that wraps a std::packaged_task. This way the
 *        threadpool can store a task with any arguments and return types.
 *        This solves problems I don't have yet and create problems that I
 *        definitely do have.
 */
template <typename Function, typename... Args>
class SpecializedTask : public Task {
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
  SpecializedTask(Function f, Args... args) {
    m_task = std::packaged_task<ReturnType()>(
        std::bind(std::forward<Function>(f), std::forward<Args>(args)...));
  }

  /**
   * @brief Desctuctor.
   */
  virtual ~SpecializedTask() {}

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
