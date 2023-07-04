#include "assetmgr.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

struct assetmgr_shm *mmap_assetmgr_shm(void) {
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

struct assetmgr_shm *init_assetmgr_shm(void) {
  int fd =
      shm_open(ASSETMGR_SHMPATH, O_CREAT | O_EXCL | O_RDWR, S_IRWXU | S_IRWXG);

  if (fd == -1) {
    shm_unlink(ASSETMGR_SHMPATH);
    return NULL;
  }

  if (ftruncate(fd, sizeof(struct assetmgr_shm)) == -1) {
    shm_unlink(ASSETMGR_SHMPATH);
    return NULL;
  }

  struct assetmgr_shm *shm =
      mmap(NULL, sizeof(*shm), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (shm == MAP_FAILED) {
    shm_unlink(ASSETMGR_SHMPATH);
    return NULL;
  }

  if (sem_init(&shm->sem, 1, 0) == -1) {
    munmap(shm, sizeof(*shm));
    shm_unlink(ASSETMGR_SHMPATH);
    return NULL;
  }

  return shm;
}

int register_asset(struct assetmgr_shm *shm, const char *filepath) {
  // TODO
}

int load_assets(struct assetmgr_shm *shm, int nassets, int *ids) {
  // TODO
}

int unload_assets(struct assetmgr_shm *shm, int nassets, int *ids) {
  // TODO
}

int assetmgrd_process_requests(struct assetmgr_shm *shm) {
    if (sem_wait(&shm->sem) == -1) {
      return -1;
    }

    int nwaiting;
    if (sem_getvalue(&shm->sem, &nwaiting) == -1) {
      return -1;
    }

    for (int i = 0; i < nwaiting; i++) {
      if (shm->requests[i].status == assetreq_status_WAITING) {
        // TODO
      }
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