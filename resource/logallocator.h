#ifndef LOGALLOCATOR_H_
#define LOGALLOCATOR_H_

#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>

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
   *
   * @param n The number of elements to allocate storage for.
   * @return A pointer to the newly allocated memory.
   */
  [[nodiscard]] T* allocate(std::size_t n) {
      LogAllocator::log<T>(n);
      return nullptr;
  }

 private:

  /**
   * @brief Static logging function for all instances of LogAllocator.
   *        Logs to a file called allocations.log the number, the
   *        size in bytes and the type of the instances separated by commas.
   * @param n The number of instances.
   */
  template <typename U>
  static void log(std::size_t n) {
    static std::ofstream of("allocations.log");
    of << n << "," << sizeof(n) << "," << typeid(U).name() << "\n";
  }
};
#endif // LOGALLOCATOR_H_
