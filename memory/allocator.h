#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include <cstddef>
#include <cstdlib>
// #include "mesh.h"

/**
 * @brief Default allocator
 */
template <typename T>
class Allocator {
 public:
  using value_type = T;
  using size_type = std::size_t;

  template <typename U>
  struct rebind {
    using other = Allocator<U>;
  };

  Allocator() = default;
  ~Allocator() = default;

  /**
   * @brief Allocate storage suitable for an array of type T[n].
   *        If the buffer is full and there are no more free objects then a new
   *        T is constructed at the back of the buffer and a pointer to it is
   *        returned.
   *
   * @param n The number of instances to allocate for.
   * @return a pointer to the allocated memory
   */
  T *allocate(size_type n) {
    return static_cast<T *>(std::malloc(n * sizeof(T)));
  }

  /**
   * @brief Deallocate memory at p.
   *
   * @param p Pointer to the memory.
   * @param n The size of the array.
   */
  void deallocate(T *p, size_type n) { std::free(p, n); }
};

// template<>
// class Allocator<Mesh> {
// };

// TODO specializations

#endif  // ALLOCATOR_H_
