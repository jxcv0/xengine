#include <assert.h>

#include "assets.h"
#include "test.h"
#include <string.h>

void tst_compress(void) {
  TEST();
  char *in = load_file("comptest.txt");
  const char *out = compress(in, strlen(in));
  printf("%s\n", out);
}

int main() {
  TEST_BEGIN();
  tst_compress();
  TEST_END();
  return 0;
}
