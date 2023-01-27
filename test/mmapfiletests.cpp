#include <gtest/gtest.h>
#include <fcntl.h>

#include <cstddef>
#include <cstring>
#include <string>

#include "mmapfile.h"

TEST(mmapfiletests, mmapfile_map) {
  struct mmapfile file = mmapfile_map("assets/models/cube/cube.obj");
  ASSERT_TRUE(file.mp_addr != NULL);
  ASSERT_EQ(file.m_size, 1076);
  mmapfile_unmap(&file);
}
