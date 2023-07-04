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
    assetmgrd_process_requests(shm);
  }

  shm_unlink(ASSETMGR_SHMPATH);
  return 0;
}
