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
 * @brief A line of text. Basically a string.
 */
struct line {
  char *mp_start;
  size_t m_len;
};

/**
 * @brief Create a memory mapped file.
 *
 * @param filepath The path to the file to open.
 * @return struct mmapfile.
 */
struct mmapfile map_file(const char *filepath);

/**
 * @brief Unmap a memory mapped file.
 *
 * @param m A pointer to the mmapfile structure.
 */
void unmap_file(struct mmapfile *mapped_file);

#ifdef __cplusplus
}
#endif

#endif  // MMAPFILE_H_
