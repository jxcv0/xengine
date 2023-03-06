#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

/**
 * @brief A circular buffer.
 */
template <typename T>
class circular_buffer {
 public:
  circular_buffer(const size_t buf_size)
      : m_index(0), m_size(0), m_max_size(buf_size) {
    mp_data = static_cast<T *>(calloc(m_max_size, sizeof(T)));
  }

  ~circular_buffer() { free(mp_data); }

  T pop() {
    assert(m_size != 0);
    uint32_t tail = tail_index();
    T temp = mp_data[tail];
    m_index = (m_index + 1) % m_max_size;
    m_size--;
    return temp;
  }

  void push(const T t) {
    assert(m_size != m_max_size);
    uint32_t head = head_index();
    mp_data[head] = t;
    m_size++;
  }

 private:
  uint32_t tail_index() {
    return m_index;
  }

  uint32_t head_index() {
    return (m_index + m_size) % m_max_size;
  }

  uint32_t m_index;
  size_t m_size;
  const size_t m_max_size;
  T *mp_data;
};

#endif  // CIRCULAR_BUFFER_H_
