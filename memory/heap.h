#ifndef HEAP_H_
#define HEAP_H_

#include <cstdint.h>

namespace xen {
using unsigned char = byte;
/**
 * @brief Initialize the preallocated heap with n bytes of memory.
 *
 * @param n The size of the heap.
 */
void heap_init(std::size_t n);

/**
 * @brief Allocate memory suitable to store an array of type T[n].
 *
 * @param n The number of elements in the array pointed to by p.
 * @return a pointer to the memory.
 */
template <typename T>
T* malloc(std::size_t n);

/**
 * @brief Free the memory allocated at p.
 *
 * @param p The pointer to the memory.
 * @param n The number of the elements pointed to by p.
 */
template <typename T>
void free(T* p, std::size_t n);
};  // namespace xen

#endif  // HEAP_H_
