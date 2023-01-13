#include "collada.h"

#include <stddef.h>
#include "xmlparser.h"

#define LIBRARY_GEOMETRIES_START "<library_geometries>"
#define LIBRARY_GEOMETRIES_END "</library_geometries>"

#define GEOMETRY_START "<geometry>"
#define GEOMETRY_END"</geometry>"

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
  return start += strlen(tag) - 1;
}

/**
 * ----------------------------------------------------------------------------
 */
char *find_geometry_elements(const char *file, char **start, char **end) {
  *start = find_xml_tag_end(file, LIBRARY_GEOMETRIES_START) + 1;
  *end = find_xml_tag_start(*start, LIBRARY_GEOMETRIES_END) - 1;
  return *start;
}
