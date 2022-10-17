#ifndef BLOCKALLOCATOR_H_
#define BLOCKALLOCATOR_H_

#include <cstdint>
#include <queue>
#include <stdexcept>
#include <memory>

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
    if (mp_mempool == nullptr) {
      mp_mempool = std::unique_ptr<T[]>(new T[nblocks]);
    }
    uintptr_t start = reinterpret_cast<uintptr_t>(mp_mempool.get());
    for (auto i = 0; i < nblocks; i++) {
      m_free_list.push(start += sizeof(T));
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
    T* ptr = nullptr;
    if (!m_free_list.empty()) {
      ptr = reinterpret_cast<T*>(m_free_list.front());
      m_free_list.pop();
    } else {
      throw std::runtime_error("out of memory");
    }
    return ptr;
  }

  /**
   * @brief Deallocate memory allocated for p.
   *
   * @param p Pointer to the memory to free.
   * @param n the size of the block to deallocate. Must always be sizeof(T)
   *
   */
  void deallocate(T* p, std::size_t n = sizeof(T)) {
    p = nullptr;
    m_free_list.push(reinterpret_cast<uintptr_t>(p));
  }

 private:
  std::queue<std::uintptr_t> m_free_list;
  std::unique_ptr<T[]> mp_mempool;
};

#endif  // BLOCKALLOCATOR_H_
