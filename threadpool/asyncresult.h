#ifndef ASYNCRESULT_H_
#define ASYNCRESULT_H_

#include <condition_variable>
#include <memory>
#include <mutex>

// shared state should be allocated elsewhere (on its own cache line?)
struct SharedState {};

// used to set the shared state where Async<T> is used to retrieve it.
// this is the obj that is left on the worker thread.
template <typename T>
struct Promise {};

/**
 * @brief Provides a mechanism to return the result of asynchronous operations
 *        between threads in syncronous looking code.
 *
 *        INCOMPLETE
 *
 * @tparam T The type of the result object.
 * @tparam Alloc The allocator to use to manage the memory required to store
 * the shared result object.
 */
template <typename T, template <typename> typename Alloc = std::allocator>
class Async {
 public:
  /**
   * @brief Allocate storage for the result object.
   */
  Async() : m_allocator() {
    m_result = std::allocate_shared<T>(m_allocator);
    m_result = nullptr;
  }

  // should this dealocate the shared pointer?
  ~Async() = default;

  void set_value(T value) {}

  /**
   * @brief Wait for the asyncronous operation to complete and get the result.
   *
   * @return A pointer to the result object.
   */
  T *await() {}

 private:
  Alloc<T> m_allocator;
  T m_result;
  std::mutex m_mutex;
  std::condition_variable m_cv;
};

#endif  // ASYNCRESULT_H_
