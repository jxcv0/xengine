#include "circular_buffer.h"

#include <assert.h>
#include <stdint.h>

int main() {
  circular_buffer<uint32_t> buf(3);
  buf.push(1);
  buf.push(2);
  buf.push(3);
  uint32_t id1 = buf.pop();
  uint32_t id2 = buf.pop();
  uint32_t id3 = buf.pop();
  assert(id1 == 1);
  assert(id2 == 2);
  assert(id3 == 3);
  buf.push(id3);
  buf.push(id2);
  buf.push(id1);
  id1 = buf.pop();
  id2 = buf.pop();
  id3 = buf.pop();
  assert(id1 == 3);
  assert(id2 == 2);
  assert(id3 == 1);
}
