#ifndef ALLOC_H
#define ALLOC_H

#include "checkerr.h"

#ifdef XEN_DEBUG
#include <string>
#endif

// TODO general heap allocator
// TODO block/pool (SLAB/SLOB?) allocator

namespace xen::mem {
using uint8 = unsigned char;

// aligned allocation
// stored alignment shift in unused memory for freeing later
template <typename T> T *alloc_aligned(size_t n) {
  // alloc
  auto align = alignof(T);
  size_t nBytes = (n * sizeof(T)) + align;
  uint8 *rawMem = new uint8[nBytes];

  // align
  const auto addr = reinterpret_cast<uintptr_t>(rawMem);

  auto mask = align - 1;
  assert((align & mask) == 0);
  auto aligned = (addr + mask) & ~mask;

  auto alignedMem = reinterpret_cast<uint8 *>(aligned);

  // shift addr if already aligned
  if (alignedMem == rawMem) {
    alignedMem += align;
  }

  // store shift
  ptrdiff_t shift = alignedMem - rawMem;
  assert(shift > 0 && shift <= 256);

  alignedMem[-1] = static_cast<uint8>(shift & 0xFF);
  return reinterpret_cast<T *>(alignedMem);
}

// fre memory using alignment shift
void free_aligned(void *ptr) {
  if (ptr) {
    auto p = reinterpret_cast<uint8 *>(ptr);

    // extract shift.
    ptrdiff_t shift = p[-1];
    if (shift == 0)
      shift = 256;

    // back up to the actual allocated address,
    // and array-delete it.
    auto rawMem = p - shift;
    delete[] rawMem;
  }
}

template <typename T> struct StackAllocator {
  // allocate aligned memory up front
  StackAllocator(size_t n) {
    size_t bytes = (n * sizeof(T)) + alignof(T);
    _start = reinterpret_cast<uintptr_t>(alloc_aligned<T>(bytes));
    _mkr = _start;
    _end = _start + bytes;
  }

  // free aligned memory
  ~StackAllocator() {

#ifdef XEN_DEBUG
    std::string msg("free: " + std::to_string(_mkr));
    logmsg(msg.c_str());
#endif

    free_aligned(reinterpret_cast<void *>(_start));
  }

  // allocate from preallocated memory
  T *allocate(size_t n) {
    auto bytes = n * sizeof(T);
    auto temp = reinterpret_cast<void *>(_mkr);
    _mkr += bytes;

    if (_mkr > _end) {
      logmsg("ERROR::OUT_OF_MEMORY");
      exit(1);
    }

#ifdef XEN_DEBUG
    std::string msg("alloc: " + std::to_string(_mkr));
    logmsg(msg.c_str());
#endif

    return static_cast<T *>(temp);
  }

  // deallocate up to a ptr
  // TODO pass in size_t n instead and free that many of T?
  void deallocate(T *ptr) {
    _mkr = reinterpret_cast<uintptr_t>(ptr);

#ifdef XEN_DEBUG
    std::string msg("dealloc:  " + std::to_string(_mkr));
    logmsg(msg.c_str());
#endif
  }

  // clear entire stack
  void clear() {
    _mkr = _start;

#ifdef XEN_DEBUG
    std::string msg("clear: " + std::to_string(_mkr));
    logmsg(msg.c_str());
#endif
  }

private:
  uintptr_t _start;
  uintptr_t _end;
  uintptr_t _mkr;
};
} // namespace xen::mem

#endif // ALLOC_H
