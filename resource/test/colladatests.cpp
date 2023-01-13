#include <cstddef>
#include <cstring>
#include <gtest/gtest.h>

#include "utils.h"
#include "collada.h"

TEST(colladatests, test) {
  char *file = load_file_into_mem("assets/models/cube/cube.dae");
  char *start, *end;
  ASSERT_NE(find_geometry_elements(file, &start, &end), nullptr);
  size_t len = end - start;
  char res[len + 1];
  res[len] = 0;
  strncpy(res, start, len);
  printf("%s\n", res);
}
