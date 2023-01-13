#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

char *find_xml_tag(const char *search_start, const char *tag);

#ifdef __cplusplus
}
#endif

#endif  // XMLPARSER_H
