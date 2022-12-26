#include <fcntl.h>
#include <gtest/gtest.h>

#include <cstddef>
#include <cstring>
#include <string>

#include "mmapfile.h"

TEST(mmapfiletests, mmapfile) {
  struct mmapfile file = map_file("assets/models/cube/cube.obj");
  ASSERT_TRUE(file.mp_addr != NULL);
  ASSERT_EQ(file.m_size, 1076);
}
