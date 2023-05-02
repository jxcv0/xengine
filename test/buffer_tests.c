#include <assert.h>

#include "buffer.h"

#define BUF_SIZE 16

struct some_struct {
  size_t s;
};

struct buffer b;

int main() {
  assert(gen_buffer(&b, BUF_SIZE, sizeof(struct some_struct)) == 0);
  assert(b.nmemb == BUF_SIZE);
  assert(b.count == 0);
  assert(b.data != NULL);
  assert(b.stride == sizeof(struct some_struct));

  struct some_struct s;

  s.s = 0;
  handle_t h1 = new_handle();
  assert(buffer_insert(&b, &s, h1) != -1);

  s.s = 1;
  handle_t h2 = new_handle();
  assert(buffer_insert(&b, &s, h2) != -1);

  s.s = 2;
  handle_t h3 = new_handle();
  assert(buffer_insert(&b, &s, h3) != -1);

  struct some_struct *data = (struct some_struct *)b.data;
  assert(data[0].s == 0);
  assert(data[1].s == 1);
  assert(data[2].s == 2);

  struct some_struct *ptr = buffer_fetch(&b, h1);
  assert(ptr != NULL);
  assert(ptr->s == 0);
  ptr = buffer_fetch(&b, h2);
  assert(ptr != NULL);
  assert(ptr->s == 1);
  ptr = buffer_fetch(&b, h3);
  assert(ptr != NULL);
  assert(ptr->s == 2);

  assert(buffer_delete(&b, h1) != -1);

  assert(data[0].s == 2);
  ptr = buffer_fetch(&b, h3);
  assert(ptr != NULL);
  assert(ptr->s == 2);

  assert(data[1].s == 1);
  ptr = buffer_fetch(&b, h2);
  assert(ptr != NULL);
  assert(ptr->s == 1);

  assert(buffer_delete(&b, h2) != -1);

  assert(data[0].s == 2);
  ptr = buffer_fetch(&b, h3);
  assert(ptr != NULL);
  assert(ptr->s == 2);

  s.s = 1;
  assert(buffer_insert(&b, &s, h1) != -1);
  assert(data[0].s == 2);
  ptr = buffer_fetch(&b, h3);
  assert(ptr != NULL);
  assert(ptr->s == 2);

  assert(data[1].s == 1);
  ptr = buffer_fetch(&b, h1);
  assert(ptr != NULL);
  assert(ptr->s == 1);
}
