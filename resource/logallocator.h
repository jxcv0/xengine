#ifndef LOGALLOCATOR_H_
#define LOGALLOCATOR_H_

#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <mutex>

/**
 * @brief Static logging function for instances of LogAllocator.
 *        Logs to a file called allocations.log the number, size of the
 *        instances in bytes, the type of the instances and the address,
 *        separated by commas.
 *
 * @param p The pointer to the newly allocated/freed memory.
 * @param n The number of instances.
 * @param alloc Flag to specify if the operation was allocating or freeing.
 */
template <typename U>
static void report_alloc(U *p, std::size_t n, bool alloc = true) {
  static std::ofstream of("allocations.log");
  static std::mutex log_mutex;
  std::lock_guard lk(log_mutex);
  of << (alloc ? "A" : "D") << "," << n << "," << sizeof(n) << ","
     << typeid(U).name() << "," << std::hex << std::showbase
     << reinterpret_cast<void *>(p) << std::dec << "\n";
}

/**
 * @brief Allocator that tracks allocations so that it can eventually be
 *        replaced with a more suitable allocator designed based on the
 *        collected data.
 *
 *        https://en.cppreference.com/w/cpp/named_req/Allocator
 */
template <typename T>
class LogAllocator {
 public:
  using value_type = T;

  LogAllocator() = default;

  /**
   * @brief Allocates storage suitable for an array object of type T[n] and
   *        creates the array, but does not construct array elements.
   *
   * @param n The number of elements to allocate storage for.
   * @return A pointer to the newly allocated memory.
   * @throws std::bad_alloc()
   */
  T *allocate(std::size_t n) {
    if (auto p = static_cast<T *>(std::malloc(n * sizeof(T)))) {
      report_alloc<T>(p, n);
      return p;
    }
    throw std::bad_alloc();
  }

  /**
   * @brief Deallocates storage pointed to p, which must be a value returned by
   * a previous call to allocate that has not been invalidated by an
   * intervening call to deallocate. n must match the value previously passed
   * to allocate.
   *
   * @param p The storage to free that was previously obtained by a call to
   *          LogAllocator::allocate.
   * @param n The number of values pointed to by p must match the value
   *          previously passed to allocate.
   */
  void deallocate(T *p, std::size_t n) noexcept {
    report_alloc(p, n, false);
    std::free(p);
  }
};

#endif  // LOGALLOCATOR_H_
