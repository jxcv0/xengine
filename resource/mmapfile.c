#include "mmapfile.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

struct mmapfile map_file(const char* filepath) {
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

void unmap_file(struct mmapfile* mmapped_file) {
  munmap(mmapped_file->mp_addr, mmapped_file->m_size);
}
