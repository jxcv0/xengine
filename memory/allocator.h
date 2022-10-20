#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include <cstddef>
#include <cstdint>
#include <memory>

/**
 * @brief Preallocated memory buffer of N bytes.
 */
template <typename T, std::size_t N>
class Allocator {
 public:

  /**
   * @brief Allocate storage suitable for an array of type T[n].
   *
   * @param n The number of instances to allocate for.
   */
  T* allocate(std::size_t n) {
  }

  void deallocate(T* p, std::size_t n) {
  }

 private:
  std::queue<uintptr_t> m_free_list;
  char m_buffer[N];
};

#endif // ALLOCATOR_H_
