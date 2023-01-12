#include "xmlparser.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/**
 * ----------------------------------------------------------------------------
 */
int parse_xml(const char *file) {
  // char *curr_tag_start;
  // char *curr_tag_end;

  char *first_tag_start = strchr(file, '<');
  first_tag_start++;
  char *first_tag_end = strchr(first_tag_start, '>');

  size_t first_tag_size = first_tag_end - first_tag_start;
  char c[first_tag_size + 1];

  strncpy(c, first_tag_start, first_tag_size);
  c[first_tag_size] = 0;
  printf("%s\n", c);

  return 0;
}
