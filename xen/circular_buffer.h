#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

#include <stdlib.h>

/**
 * @brief A circular buffer
 */
template <typename T> 
struct circular_buffer {

  circular_buffer(const size_t buf_size) : m_max_size(buf_size), m_index(0), m_size(0) {
    mp_data = static_cast<T *>(calloc(m_max_size, sizeof(T)));
  }

  ~circular_buffer() {
    free(mp_data);
  }

private:
  uint32_t m_index;
  size_t m_size;
  const size_t m_max_size;
  T *mp_data;
};

#endif // CIRCULAR_BUFFER_H_
