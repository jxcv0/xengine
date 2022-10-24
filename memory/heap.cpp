#include "heap.h"

#include <cstdlib>

// The "user space" heap
static xen::byte* s_heap;

void xen::heap_init(std::size_t n) {
  s_heap = static_cast<xen::byte*>(std::malloc(n));
}
