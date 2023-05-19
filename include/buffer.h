#ifndef BUFFER_H_
#define BUFFER_H_

#include <stddef.h>
#include <stdint.h>

struct buffer {
  struct pair {
    uint32_t handle;
    size_t offset;
  } * map;

  void *data;
  size_t nmemb;
  size_t stride;
  size_t count;
};

/**
 * @brief Set up a buffer.
 *
 * @param buf The buffer to initialize.
 * @param nmemb The number of elements in the buffer.
 * @param size The size in bytes of each member.
 * @return 0 on success. -1 on error.
 */
int buffer_init(struct buffer *buf, size_t nmemb, size_t size);

/**
 * @brief Free the resources used by buffer.
 *
 * @param buf The buffer to free.
 */
void delete_buffer(struct buffer *buf);

/**
 * @brief Insert data into a buffer and make it accessable by a handle.
 *
 * @param buf The buffer to insert the data into.
 * @param data A pointer to the data to insert.
 * @param handle The handle to associate with the data. This handle can be used
 * to retrieve the data.
 * @return 0 on success. -1 on error.
 */
int buffer_insert(struct buffer *buf, const void *data, uint32_t handle);

/**
 * @brief Delete a handle and it's associated data from a buffer.
 *
 * @param buf The buffer to delete the data from.
 * @param handle The handle to the data to delete.
 * @return 0 on success. -1 on error.
 */
int buffer_delete(struct buffer *buf, uint32_t handle);

/**
 * @brief Fetch data from a buffer using a handle.
 *
 * @param buf The buffer to fetch the data from.
 * @param handle The handle to the data to fetch.
 *
 * @return A pointer to the data associated with handle or NULL on error.
 */
void *buffer_fetch(struct buffer *buf, uint32_t handle);

#endif  // BUFFER_H_
