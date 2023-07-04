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
    int val;
    sem_getvalue(&shm->sem, &val);
    ASSERT_EQ(val, 0);
  }
  shm_unlink(ASSETMGR_SHMPATH);
}

TEST(assetmgrd_tests, register_asset) {
  struct assetmgr_shm *shm = init_assetmgr_shm();
  ASSERT_NE(shm, nullptr);
  pid_t pid = fork();
  if (pid != 0) {
    ASSERT_EQ(register_asset(shm, "assets/test/meshes/cube.obj"), 0);
  } else {
    assetmgrd_process_requests(shm);
    sem_wait(&shm->sem);
  }
  shm_unlink(ASSETMGR_SHMPATH);
}
