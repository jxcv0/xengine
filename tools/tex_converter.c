#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assets.h"
#include "stb_image.h"

void print_usage(void) {
  printf(
      "Usage:\n\t"
      "tex_converter <diffuse> <normal> <roughness> <metallic> <outfile>\n");
}

int main(int argc, const char *argv[]) {
  if (argc != 6) {
    print_usage();
    return 0;
  }

  const char *suffixes[] = {"diffuse", "normal", "roughness", "metallic"};
  for (int i = 0; i < 4; i++) {
    if (strstr(argv[i + 1], suffixes[i]) == NULL) {
      printf("Input textures are not in the correct order.\n");
      print_usage();
      exit(EXIT_FAILURE);
    }
  }

  int sizes[4][3];
  unsigned char *data[4];
  for (int i = 0; i < 4; i++) {
    data[i] = stbi_load(argv[i + 1], &sizes[i][0], &sizes[i][1], &sizes[i][2], 0);
    if (data[i] == NULL) {
      printf("Unable to load input file: '%s'.\n", argv[i]);
      exit(EXIT_FAILURE);
    }
  }

  FILE *out = fopen(argv[5], "wb+");
  if (out == NULL) {
    fprintf(stderr, "Unable to open output file '%s'.\n", argv[5]);
    exit(EXIT_FAILURE);
  }

  fprintf(out, "%d %d %d %d %d %d %d %d %d %d %d %d\n", sizes[0][0],
          sizes[0][1], sizes[0][2], sizes[1][0], sizes[1][1], sizes[1][2],
          sizes[2][0], sizes[2][1], sizes[2][2], sizes[3][0], sizes[3][1],
          sizes[3][2]);

  for (int i = 0; i < 4; i++) {
    int imglen = sizes[i][0] * sizes[i][1] * sizes[i][2];
    fwrite(data[i], sizeof(unsigned char), imglen, out);
  }

  return 0;
}
