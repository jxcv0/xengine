#include "buffer.h"

#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

/**
 * ----------------------------------------------------------------------------
 */
handle_t new_handle(void) {
  static handle_t counter = 1;
  assert(counter != INT_MAX);
  return counter++;
}

/**
 * ----------------------------------------------------------------------------
 */
int gen_buffer(struct buffer *buf, size_t nmemb, size_t stride) {
  if (nmemb == 0 || stride == 0 || buf == NULL) {
    return -1;
  }

  // TODO use mmap for these so they can be saved to file easily?
  buf->map = calloc(nmemb, sizeof(struct pair));
  buf->data = calloc(nmemb, stride);
  buf->nmemb = nmemb;
  buf->stride = stride;
  buf->count = 0;
  return 0;
}

/**
 * ----------------------------------------------------------------------------
 */
void delete_buffer(struct buffer *buf) {
  free(buf->map);
  free(buf->data);
}

/**
 * ----------------------------------------------------------------------------
 */
int buffer_insert(struct buffer *buf, const void *data, handle_t handle) {
  if (buf == NULL || data == NULL) {
    return -1;
  }

  if (buf->count >= buf->nmemb) {
    return -1;
  }

  size_t offset = buf->stride * buf->count;

  buf->map[buf->count].handle = handle;
  buf->map[buf->count].offset = offset;

  memcpy((buf->data + offset), data, buf->stride);
  ++buf->count;

  return 0;
}

/**
 * ----------------------------------------------------------------------------
 */
static int find_index(struct buffer *buf, handle_t handle, size_t *index) {
  for (size_t i = 0; i < buf->count; i++) {
    if (buf->map[i].handle == handle) {
      *index = i;
      return 0;
    }
  }
  return -1;
}

/**
 * ----------------------------------------------------------------------------
 */
static int find_last_handle(struct buffer *buf, handle_t *handle) {
  size_t last_offset = (buf->count - 1) * buf->stride;
  for (size_t i = 0; i < buf->count; i++) {
    if (buf->map[i].offset == last_offset) {
      *handle = buf->map[i].handle;
      return 0;
    }
  }
  return -1;
}

/**
 * ----------------------------------------------------------------------------
 */
int buffer_delete(struct buffer *buf, handle_t handle) {
  size_t index_to_delete;
  if (find_index(buf, handle, &index_to_delete) == -1) {
    return -1;
  }

  handle_t last_handle;
  if (find_last_handle(buf, &last_handle) == -1) {
    return -1;
  }

  size_t index_of_last;
  find_index(buf, last_handle, &index_of_last);

  if (index_to_delete == index_of_last) {
    --buf->count;
    return 0;
  }

  size_t offset_of_last = buf->map[index_of_last].offset;
  size_t offset_to_delete = buf->map[index_to_delete].offset;

  memcpy(buf->data + offset_to_delete, buf->data + offset_of_last, buf->stride);
  buf->map[index_to_delete] = buf->map[index_of_last];
  --buf->count;

  return 0;
}

/**
 * ----------------------------------------------------------------------------
 */
void *buffer_fetch(struct buffer *buf, handle_t handle) {
  size_t index;
  if (find_index(buf, handle, &index) == -1) {
    return NULL;
  }
  return buf->data + buf->map[index].offset;
}
