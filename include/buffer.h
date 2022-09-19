#ifndef BUFFER_H
#define BUFFER_H

#include "alloc.h"

namespace xen {
// circular fifo queue
template <typename T> struct CircularBuffer {
  CircularBuffer(size_t n) {
    _maxSize = n;
    _buffer = mem::alloc_aligned<T>(_maxSize);
  }

  ~CircularBuffer() { mem::free_aligned(_buffer); }

  inline void write(T item) {
    _buffer[_write] = item;
    _write = (_write + 1) % _maxSize;
    _size++;
  }

  inline T read() {
    auto temp = _read;
    _read = (_read + 1) % _maxSize;
    _size--;
    return _buffer[temp];
  }

  inline T *readptr() {
    auto temp = _read;
    _read = (_read + 1) % _maxSize;
    _size--;
    return &_buffer[temp];
  }

  // get size
  auto size() { return _size; }

private:
  size_t _maxSize;
  size_t _read, _write = 0;
  T *_buffer;
  size_t _size = 0;
};
} // namespace xen
#endif // BUFFER_H
