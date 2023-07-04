#include <gtest/gtest.h>

#include "assetmgr.h"

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

TEST(assetmgrd_tests, init_assetmgr_shm) {
  struct assetmgr_shm *shm = init_assetmgr_shm();
  ASSERT_NE(shm, nullptr);
  pid_t pid = fork();
  if (pid != 0) {
  } else {
    int val;
    sem_getvalue(&shm->sem, &val);
    ASSERT_EQ(val, 0);
  }
  shm_unlink(ASSETMGR_SHMPATH);
}
