#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include <cstddef>
#include <memory>

/**
 * @brief Preallocated memory buffer.
 */
template <typename T, std::size_t N>
class Allocator {
 public:
  T* allocate(std::size_t n) {
  }

  void deallocate(T* p, std::size_t n) {
  }

 private:
   char m_buffer[N];
};

#endif // ALLOCATOR_H_
