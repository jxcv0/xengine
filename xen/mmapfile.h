#ifndef MMAPFILE_H_
#define MMAPFILE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/**
 * @brief Memory mapped file data;
 */
struct mmapfile {
  void *mp_addr;
  size_t m_size;
};

/**
 * @brief Map a file to a memory address
 *
 * @param filepath The path to the file to open.
 * @return An array of the bytes in the file.
 */
char *map_file(size_t *n, const char *filepath);

/**
 * @brief Unmap a memory mapped file.
 *
 * @param file A pointer to the mapped address.
 * @param n The size of the mapped file in bytes.
 */
void unmap_file(char *file, const size_t n);

#ifdef __cplusplus
}
#endif

#endif  // MMAPFILE_H_
