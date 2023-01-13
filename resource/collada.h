#ifndef COLLADA_H_
#define COLLADA_H_

#ifdef __cplusplus
extern "C" {
#endif

void parse_collada_file(const char *filepath);

char *find_next_tag_start(const char *haystack);

#ifdef __cplusplus
}
#endif

#endif  // COLLADA_H_
