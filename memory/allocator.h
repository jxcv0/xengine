#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <new>
#include <queue>

/**
 * @brief Allocator with dynamically sized object pool.
 */
template <typename T>
class Allocator {
 public:
  using value_type = T;

  /**
   * @brief Construct an Allocator<T> object.
   *
   * @param n The initial size of the object pool.
   */
  Allocator() : m_capacity(1), m_size(0) {
    m_object_pool = static_cast<T*>(std::calloc(m_capacity, sizeof(T)));
    for (auto i = 0; i < m_capacity; i++) {
      auto addr = reinterpret_cast<std::uintptr_t>(&m_object_pool[i]);
      m_free_list.push(addr);
    }
  }

  ~Allocator() { std::free(m_object_pool); }

  /**
   * @brief Allocate storage suitable for an array of type T[n].
   *        If the buffer is full and there are no more free objects then a new
   *        T is constructed at the back of the buffer and a pointer to it is
   *        returned.
   *
   * @param n The number of instances to allocate for.
   * @return a pointer to the allocated memory
   */
  T* allocate(std::size_t n) {
    T* p = nullptr;
    if (m_size == m_capacity) {
      resize();
    }
    p = reinterpret_cast<T*>(m_free_list.front());
    m_size++;
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
      m_free_list.push(reinterpret_cast<uintptr_t>(&p[i]));
    }
  }

  /**
   * @brief Get the current capacity of the Allocators object pool.
   *
   * @return The current capacity of the object pool.
   */
  std::size_t capacity() { return m_capacity; }

  /**
   * @brief Rebind for allocating multiple types
   *        i.e std::vector<std::string, Allocator<std::string>>.
   */
  template <typename U>
  struct rebind {
    using other = Allocator<U>;
  };

 private:
  /**
   * @brief Search for a block of contiguous memory large enough to store an
   *        array of type T[n]
   */
  T* find_contiguous(std::size_t n) {
    // TODO
    return nullptr;
  }

  void resize() {
    m_capacity *= 2;
    auto new_mem = std::realloc(m_object_pool, m_capacity);
    if (new_mem != nullptr) {
      m_object_pool = static_cast<T*>(new_mem);
    } else {
      throw std::bad_alloc();
    }
    for (auto i = m_size; i < m_capacity; i++) {
      auto addr = reinterpret_cast<std::uintptr_t>(&m_object_pool[i]);
      m_free_list.push(addr);
    }
  }

  std::queue<uintptr_t> m_free_list;
  T* m_object_pool;
  std::size_t m_capacity;
  std::size_t m_size;
};

#endif  // ALLOCATOR_H_
