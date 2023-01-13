#include "collada.h"

#include <stddef.h>
#include <string.h>

#define LIBRARY_GEOMETRIES_START "<library_geometries>"
#define LIBRARY_GEOMETRIES_END "</library_geometries>"

#define GEOMETRY_START "<geometry>"
#define GEOMETRY_END "</geometry>"

#define MESH_START "<mesh>"
#define MESH_END "</mesh>"

/**
 * ----------------------------------------------------------------------------
 */
char *find_xml_tag_start(const char *search_start, const char *tag) {
  return strstr(search_start, tag);
}

/**
 * ----------------------------------------------------------------------------
 */
char *find_xml_tag_end(const char *search_start, const char *tag) {
  char *start = strstr(search_start, tag);
  return start += strlen(tag);
}

/**
 * ----------------------------------------------------------------------------
 */
char *find_geometry_elements(const char *file, char **start, char **end) {
  *start = find_xml_tag_start(file, LIBRARY_GEOMETRIES_START);
  *end = find_xml_tag_end(*start, LIBRARY_GEOMETRIES_END);
  return *start;
}
