#include "collada.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

#define COLLADA_START "COLLADA"

#define TAG_START '<'
#define TAG_END '>'

#define LIBRARY_GEOMETRIES_START "<library_geometries>"
#define LIBRARY_GEOMETRIES_END "</library_geometries>"

void put(const char *start, const char *end) {
  for (const char *c = start; c <= end; c++) {
    putchar(*c);
  }
  putchar('|');
  putchar('\n');
}

static char *handle_tag(const char *tag_start, const char *tag_end);
static char *handle_geometries(const char *geometries_start,
                               const char *geometries_end);

/**
 * -----------------------------------------------------------------------------
 */
void parse_collada_file(const char *file) {
  char *tag_start = strchr(file, TAG_START);
  while ((tag_start = strchr(tag_start, TAG_START))) {
    char *tag_end = strchr(tag_start, TAG_END);
    tag_start = handle_tag(tag_start, tag_end);
    tag_start++;
    // break;
  }
}

/**
 * -----------------------------------------------------------------------------
 */
static char *handle_tag(const char *tag_start, const char *tag_end) {
  size_t n = tag_end - tag_start + 1;

  if (strncmp(tag_start, LIBRARY_GEOMETRIES_START, n) == 0) {
    // find end of geomentries module
    char *geometries_end = strstr(tag_start, LIBRARY_GEOMETRIES_END);
    return handle_geometries(tag_start, geometries_end);
  }
  return (char *)tag_start;
}

/**
 * -----------------------------------------------------------------------------
 */
static char *handle_geometries(const char *geometries_start,
                               const char *geometries_end) {
  char *tag_start = strchr(geometries_start + 1, TAG_START);
  while ((tag_start = strchr(tag_start, TAG_START)) != geometries_end) {
    char *tag_end = strchr(tag_start, TAG_END);

    size_t n = tag_end - tag_start + 1;
    put(tag_start, tag_end);
    if (strncmp(tag_start, "geometry", n) == 0) {
    }

    tag_start++;
  }
  return (char *)(geometries_end + strlen(LIBRARY_GEOMETRIES_END)); // this might need to be -1
}
