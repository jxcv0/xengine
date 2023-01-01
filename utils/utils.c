#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * ----------------------------------------------------------------------------
 */
char *load_file_into_mem(const char *filepath) {
  FILE *file = fopen(filepath, "r");
  fseek(file, 0, SEEK_END);
  long filesize = ftell(file);
  char *buff = malloc(filesize + 1);  // + 1 for '\0'
  rewind(file);

  size_t nread = fread(buff, filesize, 1, file);
  (void)nread;

  fclose(file);
  buff[filesize] = '\0';

  return buff;
}

/**
 * ----------------------------------------------------------------------------
 */
unsigned char *load_file_into_mem_u(const char *filepath) {
  FILE *file = fopen(filepath, "r");
  fseek(file, 0, SEEK_END);
  long filesize = ftell(file);
  unsigned char *buff = malloc(filesize + 1);  // + 1 for '\0'
  rewind(file);

  size_t nread = fread(buff, filesize, 1, file);
  (void)nread;

  fclose(file);
  buff[filesize] = '\0';

  return buff;
}
