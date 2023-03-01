#ifndef UTILS_H_
#define UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Open and read a file into memory. The returned memory must be freed.
 *
 * @param filepath The path to the file to open.
 * @return The contents of the file as a char array.
 */
char *load_file_into_mem(const char *filepath);

/**
 * @brief Open and read a file into memory. The returned memory must be freed.
 *
 * @param filepath The path to the file to open.
 * @return The contents of the file as a char array.
 */
unsigned char *load_file_into_mem_u(const char *filepath);

char *dirname(char *buff, char *src);

#ifdef __cplusplus
}
#endif

#endif  // UTILS_H_
