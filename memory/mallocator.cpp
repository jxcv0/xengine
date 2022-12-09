#include "mallocator.h"

template <>
void *Mallocator<void>::allocate(std::size_t n) {
  return std::malloc(n);
}
