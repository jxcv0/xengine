#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "assets.h"
#include "test.h"

void tst_compress(void) {
  TEST();
  size_t size;
  // char *in = load_file_size("assets/textures/ravine_rock", &size);
  char *in = load_file_size("assets/textures/ravine_rock", &size);
  size_t ntoks;
  struct lz77tok *out = compress_lz77(in, size, &ntoks);
  char *decomp = decompress_lz77(out, ntoks, size);
  // printf("%s\n%s\n", in, decomp);
  FILE *f = fopen("test.mtl", "wb+");
  fwrite(decomp, sizeof(char), size, f);
  // assert(memcmp(in, decomp, size) == 0);
  printf("%ld bytes -> %ld bytes (%ld tokens)\n", size,
         ntoks * sizeof(struct lz77tok), ntoks);
  free(out);
  free(decomp);
}

int main() {
  TEST_BEGIN();
  tst_compress();
  TEST_END();
  return 0;
}
