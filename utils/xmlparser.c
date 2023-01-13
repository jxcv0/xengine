#include "xmlparser.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/**
 * ----------------------------------------------------------------------------
 */
char *find_xml_tag(const char *search_start, const char *tag) {
  return strstr(search_start, tag);
}
