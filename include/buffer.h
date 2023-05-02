#ifndef BUFFER_H_
#define BUFFER_H_

#include <stddef.h>
#include <stdint.h>

typedef int32_t handle_t;

struct buffer {
  struct pair {
    handle_t handle;
    size_t offset;
  } * map;

  void *data;
  size_t size;
  size_t stride;
  size_t count;
};

handle_t new_handle(void);

int gen_buffer(struct buffer *buf, size_t size, size_t stride);

void delete_buffer(struct buffer *buf);

int buffer_insert(struct buffer *buf, const void *data, handle_t handle);

int buffer_delete(struct buffer *buf, handle_t handle);

void *buffer_fetch(struct buffer *buf, handle_t handle);

#endif  // BUFFER_H_
