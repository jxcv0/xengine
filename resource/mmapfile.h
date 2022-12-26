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
struct string {
  char *mp_start;
  size_t m_len;
};

/**
 * @brief Create a memory mapped file.
 *
 * @param filepath The path to the file to open.
 * @return struct mmapfile.
 */
struct mmapfile mmapfile_map(const char *filepath);

/**
 * @brief Unmap a memory mapped file.
 *
 * @param m A pointer to the mmapfile structure.
 */
void mmapfile_unmap(struct mmapfile *mapped_file);

/**
 * @brief Find the next occurence of a character in a memory mapped file
 * starting at position pos.
 *
 * @param mapped_file A pointer to the mmapfile structure.
 * @param pos The starting index of the search.
 * @param c The character to search for.
 * @return A string structure.
 */
struct string mmapfile_find(struct mmapfile *mapped_file, size_t pos, char c);

#ifdef __cplusplus
}
#endif

#endif  // MMAPFILE_H_
