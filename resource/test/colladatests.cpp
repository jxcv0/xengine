#include <cstddef>
#include <cstring>
#include <gtest/gtest.h>

#include "utils.h"
#include "mmapfile.h"
#include "collada.h"

TEST(colladatests, normal_file) {
  const char *file = load_file_into_mem("assets/models/cube/cube.dae");
  parse_collada_file(file);
  free((void *)file);
}
