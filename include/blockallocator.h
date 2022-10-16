#ifndef BLOCKALLOCATOR_H_
#define BLOCKALLOCATOR_H_

#include <cstdint>
#include <cstdlib>
#include <list>

/**
 * @brief Allocate fixed size blocks of memory
 */
template <typename T>
class BlockAllocator {
  using value_type = T;

 public:
  /**
   * @brief Construct a block allocator.
   *
   * @param nblocks Allocate enough memory to store nblock instances of T.
   */
  BlockAllocator(std::size_t nblocks) {
    if (s_mempool == nullptr) {
      s_mempool = static_cast<T*>(std::calloc(nblocks, sizeof(T)));
    } else if (m_free_list.size() < nblocks) {
      s_mempool = static_cast<T*>(
          std::realloc(s_mempool, m_free_list.size() + nblocks));
    }
  }

  ~BlockAllocator() = default;

  /**
   * @brief Allocate memory for n number of instances of T.
   *
   * @param n The number of blocks to allocate
   * @return A pointer to the new allocated blocks
   */
  T* allocate(std::size_t n = sizeof(T)) {
    // TODO
    return nullptr;
  }

  /**
   * @brief Deallocate memory allocated for p.
   *
   * @param p Pointer to the memory to free.
   * @param n the size of the block to deallocate. Must always be sizeof(T)
   *
   */
  void deallocate(T* p, std::size_t n = sizeof(T)) {
    // TODO
  }

  /**
   * @brief Free all blocks of memory.
   */
  void deallocate_all() {
    // TODO
  }

 public:
  std::list<std::uintptr_t> m_free_list;
  static T* s_mempool;
};

#endif  // BLOCKALLOCATOR_H_
