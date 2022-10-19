#ifndef BLOCKALLOCATOR_H_
#define BLOCKALLOCATOR_H_

#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <queue>
#include <stdexcept>

/**
 * @brief Fixed size static memory pool
 */
template <typename T, size_t N>
class PoolAllocator {
  using value_type = T;

 public:
  /**
   * @brief Construct a pool allocator.
   */
  PoolAllocator() {
    for (auto i = 0; i < N; i++) {
      auto ptr = reinterpret_cast<uintptr_t>(&s_object_pool[i]);
      m_free_list.push(ptr);
    }
  }

  ~PoolAllocator() = default;

  /**
   * @brief Allocate memory for n number of instances of T.
   *
   * @return A pointer to a T
   */
  T* allocate(size_t n = sizeof(T)) {
    assert(n == sizeof(T));
    if (m_free_list.empty()) {
      throw std::runtime_error("out of memory");
    }
    T* ptr = reinterpret_cast<T*>(m_free_list.front());
    m_free_list.pop();
    return ptr;
  }

  /**
   * @brief Deallocate memory allocated for p and put address in free list.
   *
   * @param p Pointer to the object to deallocate.
   *
   */
  void deallocate(T* p, size_t n = sizeof(T)) {
    assert(n == sizeof(T));
    p = nullptr;
    m_free_list.push(reinterpret_cast<uintptr_t>(p));
  }

 private:
  std::queue<std::uintptr_t> m_free_list;
  T s_object_pool[N];
};

#endif  // BLOCKALLOCATOR_H_
