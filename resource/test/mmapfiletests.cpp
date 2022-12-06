#include <cstddef>
#include <cstring>
#include <fcntl.h>
#include <gtest/gtest.h>
#include <string>
#include "mmapfile.h"

TEST(mmapfiletests, mmapfile) {
  MmapFile file("assets/models/cube/cube.obj");
  ASSERT_TRUE(file.valid());
  auto c = file.data();
  auto nl = static_cast<const char *>(memchr(c, '\n', file.len()));
  const std::string expected = "# Blender v2.76 (sub 0) OBJ File: ''";
  auto size = nl - c;
  char actual[size];
  strncpy(actual, c, size);
  actual[size] = '\0';
  ASSERT_STREQ(actual, expected.c_str());
}
