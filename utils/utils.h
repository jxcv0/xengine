#ifndef UTILS_H_
#define UTILS_H_

/**
 * @brief Open and read a file into memory. The returned memory must be freed.
 *
 * @param filepath The path to the file to open.
 * @return The contents of the file as a char array.
 */
char *load_file_into_mem(const char *filepath);

#endif  // UTILS_H_
