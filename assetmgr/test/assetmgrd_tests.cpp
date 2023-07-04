#include <gtest/gtest.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "assetmgr.h"

TEST(assetmgrd_tests, init_assetmgr_shm) {
  struct assetmgr_shm *shm = init_assetmgr_shm();
  ASSERT_NE(shm, nullptr);
  pid_t pid = fork();
  if (pid != 0) {
    int val;
    sem_getvalue(&shm->mgrsem, &val);
    ASSERT_EQ(val, 0);
    sem_getvalue(&shm->reqsem, &val);
    ASSERT_EQ(val, 1);
  }
  shm_unlink(ASSETMGR_SHMPATH);
}

TEST(assetmgrd_tests, register_asset) {
  struct assetmgr_shm *shm = init_assetmgr_shm();
  ASSERT_NE(shm, nullptr);
  pid_t pid = fork();
  if (pid != 0) {
    int id = register_asset(shm, "assets/test/meshes/cube.obj");
    sem_wait(&shm->reqsem);
    ASSERT_STREQ(asset_path(shm, id), "assets/test/meshes/cube.obj");
  } else {
    assetmgrd_process_requests(shm);
  }
  shm_unlink(ASSETMGR_SHMPATH);
}
