#include "collada.h"
#include "utils.h"

#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#define COLLADA_START "COLLADA"

#define TAG_START '<'

#define LIBRARY_GEOMETRIES_START "<library_geometries>"
#define LIBRARY_GEOMETRIES_END "</library_geometries>"

#define GEOMETRY_START "<geometry>"
#define GEOMETRY_END "</geometry>"

#define MESH_START "<mesh>"
#define MESH_END "</mesh>"

/**
 * ----------------------------------------------------------------------------
 */
void parse_collada_file(const char *file) {
  char *tag_start = strchr(file, TAG_START);
  while((tag_start = strchr(tag_start, TAG_START))) {
    tag_start++;
  }
}

