#include "mmapfile.h"

#include <assert.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

/**
 * ----------------------------------------------------------------------------
 */
char *map_file(size_t *n, const char *filepath) {
  int fd = open(filepath, O_RDWR);
  assert(fd != -1);
  *n = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);
  char *file =
      (char *)mmap(NULL, *n, (PROT_READ | PROT_WRITE), MAP_PRIVATE, fd, 0);
  close(fd);
  return file;
}

/**
 * ----------------------------------------------------------------------------
 */
void unmap_file(char *file, const size_t n) { munmap((void *)file, n); }
