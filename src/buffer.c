#include "buffer.h"

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

handle_t new_handle(void) {
    static handle_t counter = 1;
    assert(counter != INT_MAX);
    return counter++;
}

int gen_buffer(struct buffer *buf, size_t size, size_t stride) {
  if (size == 0 || stride == 0 || buf == NULL) {
    return -1;
  }

  buf->map = calloc(size, sizeof(struct pair));
  buf->data = calloc(size, stride);
  buf->size = size;
  buf->stride = stride;
  buf->count = 0;
  return 0;
}

void delete_buffer(struct buffer *buf) {
  free(buf->map);
  free(buf->data);
}

int buffer_insert(struct buffer *buf, const void *data, handle_t handle) {
  if (buf == NULL || data == NULL) {
    return -1;
  }

  if (buf->count >= buf->size) {
    return -1;
  }

  size_t index = buf->count++;

  buf->map[index].handle = handle;
  buf->map[index].index = index;

  memcpy((buf->data + (index * buf->stride)), data, buf->stride);

  return 0;
}

static int find_index(struct buffer *buf, handle_t handle, size_t *index) {
  for (size_t i = 0; i < buf->count; i++) {
    if (buf->map[i].handle == handle) {
      *index = i;
      return 0;
    }
  }
  return -1;
}

static int find_last_handle(struct buffer *buf, handle_t *handle) {
  size_t last_index = buf->count - 1;
  for (size_t i = 0; i < buf->count; i++) {
    if (buf->map[i].index == last_index) {
      *handle = buf->map[i].handle;
      return 0;
    }
  }
  return -1;
}

int buffer_delete(struct buffer *buf, handle_t handle) {
  size_t index;
  if (find_index(buf, handle, &index) == -1) {
    return -1;
  }

  size_t offset_to_delete = buf.map[index].index * buf->stride;
  size_t offset_last = (buf->count - 1) * buf->stride;

  if (offset_to_delete == offset_last) {
    --buf->count;
    return 0;
  }

  handle_t last_handle;
  if (find_last_handle(buf, &last_handle) == -1) {
    return -1;
  }
  
  // ...

  return 0;
}
