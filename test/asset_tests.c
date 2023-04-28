#include <string.h>

#include "assets.h"

#define TEST_MAX_GEOMS 32

struct geometry arr[TEST_MAX_GEOMS] = {0};
size_t table[TEST_MAX_GEOMS];
size_t count = 0;

int main() {
  memset(table, -1, sizeof(table));
  return 0;
}
