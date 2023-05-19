#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assets.h"
#include "stb_image.h"

void print_usage(void) {
  printf(
      "Usage:\n\t"
      "tex_converter <infile> <outfile>\n");
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    print_usage();
    return 0;
  }
  int w, h, n;
  unsigned char *data = stbi_load(argv[1], &w, &h, &n, 0);

  if (data == NULL) {
    fprintf(stderr, "Unable to load input file '%s'.\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  FILE *out = fopen(argv[2], "wb+");
  if (out == NULL) {
    fprintf(stderr, "Unable to open output file '%s'.\n", argv[2]);
    exit(EXIT_FAILURE);
  }

  fprintf(out, "%d %d %d\n", w, h, n);
  int imglen = w * h * n;
  fwrite(data, sizeof(unsigned char), imglen, out);
  fclose(out);
  return 0;
}
