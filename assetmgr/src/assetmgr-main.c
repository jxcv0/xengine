#define _POSIX_C_SOURCE 200112L
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "assetmgr.h"

#define handle_error_en(en, msg) \
  do {                           \
    errno = en;                  \
    perror(msg);                 \
    exit(EXIT_FAILURE);          \
  } while (0)

#define handle_error(msg) \
  do {                    \
    perror(msg);          \
    exit(EXIT_FAILURE);   \
  } while (0)

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  int fd =
      shm_open(ASSETMGR_SHMPATH, O_CREAT | O_EXCL | O_RDWR, S_IRWXU | S_IRWXG);

  if (fd == -1) {
    handle_error("shm_open");
  }

  if (ftruncate(fd, sizeof(struct assetmgr_shm)) == -1) {
    handle_error("ftruncate");
  }

  struct assetmgr_shm *shm =
      mmap(NULL, sizeof(*shm), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (shm == MAP_FAILED) {
    handle_error("mmap");
  }

  if (sem_init(&shm->sem, 1, 0) == -1) {
    handle_error("sem_init");
  }

  for (int i = 0; i < MAX_NUM_REQUESTS; i++) {
    if (sem_init(&shm->requests[i].sem, 1, 0) == -1) {
      handle_error("sem_init");
    }
  }

  while (1) {
    if (sem_wait(&shm->sem) == -1) {
      handle_error("sem_wait");
    }

    // this semaphore is only waited on by this process so we will always get
    // the value.
    int nwaiting;
    if (sem_getvalue(&shm->sem, &nwaiting) == -1) {
      handle_error("sem_getvalue");
    }

    for (int i = 0; i < nwaiting; i++) {
      int val;
      if (sem_getvalue(&shm->requests[i].sem, &val) == -1) {
        handle_error("sem_getvalue");
      }

      // this request is waiting.
      if (val == 0) {
      }
    }
  }

  shm_unlink(ASSETMGR_SHMPATH);
  return 0;
}
