#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "assets.h"

#define TEST_TABLE_SIZE 16

struct geometry arr[TEST_TABLE_SIZE];
struct handle_index_pair map[TEST_TABLE_SIZE];
size_t count = 0;

int main() {
  handle_t h1 = new_asset_handle();
  assert(provision_geometry(arr, map, TEST_TABLE_SIZE, &count, h1) == 0);
}
