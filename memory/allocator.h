#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <new>
#include <queue>

/**
 * @brief Preallocated memory buffer.
 */
template <typename T, std::size_t N>
class Allocator {
 public:
  
  /**
   * @brief Construct an Allocator<T, N> instance.
   */
  Allocator() {
    uintptr_t start = reinterpret_cast<uintptr_t>(&m_buffer[0]);
    auto bufsize = sizeof(T) * N;
    for (auto i = 0; i < N; i++) {
      m_free_list.push(start);
      start += sizeof(T);
    }
  }

  /**
   * @brief Allocate storage suitable for an array of type T[n].
   *
   * @param n The number of instances to allocate for.
   */
  T* allocate(std::size_t n) {
    if (m_free_list.empty()) {
      throw std::bad_alloc();
    }
    T *p = reinterpret_cast<T*>(m_free_list.front());
    m_free_list.pop();
    return p;
  }

  /**
   * @brief Deallocate memory at p.
   * 
   * @param p Pointer to the memory.
   * @param n The size of the array.
   */
  void deallocate(T* p, std::size_t n) {
    for (auto i = 0; i < n; i++) {
      m_free_list.push(reinterpret_cast<uintptr_t>(p[i]));
      p[i] = nullptr;
    }
  }

 private:
  std::queue<uintptr_t> m_free_list;
  char m_buffer[sizeof(T) * N];
};

#endif // ALLOCATOR_H_
