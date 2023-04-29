#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "assets.h"

#define TABLE_SIZE 16
handle_t table[TABLE_SIZE];
handle_t expected_table[] = {15, 14, 13, 12, 11, 10, 9, 8,
                             7,  6,  5,  4,  3,  2,  1, 0};
handle_t final_table[] = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 0, 1, 2};

struct handle_table ht = {
    .table = table,
    .size = TABLE_SIZE,
};

int main() {
  init_handle_table(&ht);

  for (size_t i = 0; i < ht.size; i++) {
    assert(ht.table[i] == expected_table[i]);
  }

  handle_t h1;
  assert(get_handle(&ht, &h1) != -1);
  assert(h1 == 0);
  assert(ht.count == 15);

  handle_t h2;
  assert(get_handle(&ht, &h2) != -1);
  assert(h2 == 1);
  assert(ht.count == 14);

  handle_t h3;
  assert(get_handle(&ht, &h3) != -1);
  assert(h3 == 2);
  assert(ht.count == 13);

  assert(release_handle(&ht, 11) == -1);

  assert(release_handle(&ht, h1) != -1);
  assert(ht.count == 14);

  assert(release_handle(&ht, h2) != -1);
  assert(ht.count == 15);

  for (size_t i = 0; i < ht.count; i++) {
    printf("%ld\n", ht.table[i]);
  }

  assert(release_handle(&ht, h3) != -1);
  assert(ht.count == 16);

  for (size_t i = 0; i < ht.size; i++) {
    assert(ht.table[i] == final_table[i]);
  }
}
