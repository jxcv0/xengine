#ifndef MALLOCATOR_H_
#define MALLOCATOR_H_

#include <cstddef>
#include <cstdlib>

template <typename T>
struct Mallocator {
  using value_type = T;

  T *allocate(std::size_t n) {
    return static_cast<T *>(std::malloc(n * sizeof(T)));
  }

  void deallocate(T *p, [[maybe_unused]] std::size_t n) { std::free(p); }
};

template <>
void *Mallocator<void>::allocate(std::size_t n);

#endif  // MALLOCATOR_H_
