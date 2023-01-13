#include <cstdio>
#include <gtest/gtest.h>

#include "utils.h"
#include "xmlparser.h"
#include <cstdlib>

TEST(xmlparsertests, test) {
  char *file = load_file_into_mem("assets/models/cube/cube.dae");
  char *tag = find_xml_tag(file, "<library_geometries>");
  ASSERT_NE(tag, nullptr);
  free(file);
}
