#include "assetmgr.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

struct assetmgr_shm *create_assetmgr_shm(void) {
  int fd = shm_open(ASSETMGR_SHMPATH, O_RDWR, 0);

  if (fd == -1) {
    return NULL;
  }

  struct assetmgr_shm *shm =
      mmap(NULL, sizeof(*shm), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (shm == MAP_FAILED) {
    return NULL;
  }
}

int asset_type(const char *path) {
  char *ext = strrchr(path, '.');

  if (ext == NULL) {
    return -1;
  }

  const char *supported_asset_exts[] = {".mesh", ".mtl"};
  for (int i = 0; i < 2; i++) {
    if (strcmp(supported_asset_exts[i], ext) == 0) {
      return i;
    }
  }

  return -1;
}