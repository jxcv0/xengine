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

  struct assetmgr_shm *shm = init_assetmgr_shm();

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
      if (shm->requests[i].status == assetreq_status_WAITING) {

      }
    }
  }

  shm_unlink(ASSETMGR_SHMPATH);
  return 0;
}
