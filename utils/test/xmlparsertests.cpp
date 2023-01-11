#include <gtest/gtest.h>

#include "utils.h"
#include "xmlparser.h"

TEST(xmlparsertests, test) {
  const char *file = load_file_into_mem("assets/models/cube/cube.dae");
  struct xml_file result;
  xmlparser_parse(&result, file);
}
