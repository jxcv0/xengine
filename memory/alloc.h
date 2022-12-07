#ifndef ALLOC_H_
#define ALLOC_H_

#include <cstdint>
#include <cstdlib>

namespace xen {

template <typename T>
T *alloc(const std::size_t n) {
  return static_cast<T *>(std::malloc(sizeof(T) * n));
}

template <typename T>
void free(T *p) {}

}  // namespace xen

#endif  // ALLOC_H_
