#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Parse an xml file.
 */
int parse_xml(const char *file);

#ifdef __cplusplus
}
#endif

#endif  // XMLPARSER_H
