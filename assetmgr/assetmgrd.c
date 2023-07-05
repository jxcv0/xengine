#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h> // for shm_unlink

#include "assetmgr.h"

#define handle_error(msg)                                                     \
  do                                                                          \
    {                                                                         \
      perror (msg);                                                           \
      exit (EXIT_FAILURE);                                                    \
    }                                                                         \
  while (0)

char *filepaths[MAX_NUM_REQUESTS];
int nreg = 0;

/* assetmgrd - game asset manager */
int
main (int argc, char **argv)
{
  (void)argc;
  (void)argv;

  struct assetmgr_shm *shm = init_assetmgr_shm ();
  if (!shm)
    {
      handle_error ("init_assetmgr_shm");
    }

  while (1)
    {
      assetmgrd_process_requests (shm);
    }

  shm_unlink (ASSETMGR_SHMPATH);
  return 0;
}
