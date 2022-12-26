#include <fcntl.h>
#include <gtest/gtest.h>

#include <cstddef>
#include <cstring>
#include <string>

#include "mmapfile.h"

TEST(mmapfiletests, mmapfile) {
  struct mmapfile file = map_file("assets/models/cube/cube.obj");
  ASSERT_TRUE(file.mp_addr != NULL);
  // auto view = file.view();
  // auto sub = view.substr(0, view.find('\n'));
  // const std::string expected = "# Blender v2.76 (sub 0) OBJ File: ''";
  // auto size = sub.size();
  // char actual[size];
  // strncpy(actual, sub.data(), size);
  // actual[size] = '\0';
  // ASSERT_STREQ(actual, expected.c_str());
}
