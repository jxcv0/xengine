#include "mmapfile.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

/**
 * ----------------------------------------------------------------------------
 */
struct mmapfile mmapfile_map(const char *filepath) {
  struct mmapfile result = {.mp_addr = NULL, .m_size = 0};

  int fd = open(filepath, O_RDWR);
  if (fd == -1) {
    return result;
  }

  result.m_size = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);
  result.mp_addr =
      mmap(NULL, result.m_size, (PROT_READ | PROT_WRITE), MAP_PRIVATE, fd, 0);
  close(fd);
  return result;
}

/**
 * ----------------------------------------------------------------------------
 */
void mmapfile_unmap(struct mmapfile *mmapped_file) {
  munmap(mmapped_file->mp_addr, mmapped_file->m_size);
}

/**
 * ----------------------------------------------------------------------------
 */
struct string mmapfile_find(struct mmapfile *mapped_file, size_t pos, char c) {
  struct string string = {.mp_start = NULL, .m_len = 0};
  char *file = (char *)mapped_file->mp_addr;
  // TODO continue
  string.mp_start = (char *) memchr(&file[pos], c, mapped_file->m_size - pos);
  return string;
}
