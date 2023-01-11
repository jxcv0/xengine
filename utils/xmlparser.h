#ifndef XMLPARSER_H
#define XMLPARSER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Parse an xml file. The file is assumed to be a NULL terminated string.
 */
int xmlparser_parse(const char *file);

#ifdef __cplusplus
}
#endif

#endif  // XMLPARSER_H
