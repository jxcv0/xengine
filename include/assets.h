#ifndef ASSETS_H_
#define ASSETS_H_

#include "components.h"
#include "glad.h"
#include "lin.h"

#define TEXTURE_DIR "assets/textures/"
#define COMP_WINDOW_LEN 64
#define MAX_LOADED_ASSETS 32

struct lz77tok {
  uint8_t start;
  uint8_t len;
  char next;
};

/**
 * @brief Load a file from disk into memory.
 *
 * @param filepath The path of the file to load.
 * @return A null terminated string of the file contents.
 */
char *load_file(const char *filepath);

char *load_file_size(const char *filepath, size_t *len);

struct lz77tok *compress_lz77(const char *data, size_t len, size_t *ntoks);

char *decompress_lz77(struct lz77tok *toks, size_t ntoks, size_t n);

/**
 * TODO
 */
int write_file(char *bytes, const char *filepath);

/**
 * @brief Load a 3D object from a xengine binary file.
 * @param geom The geometry structure to store the buffer object handles.
 * @param filepath The filepath to load the object from.
 * @return 0 on success, -1 on failure.
 */
int load_obj(struct mesh *mesh, const char *filepath);

/**
 * @brief Load a material from a xengine binary file.
 * @param mat The material structure to store the image buffer handles.
 * @param filepath The file to load the material from.
 * @return 0 on success, -1 on failure.
 */
int load_mtl(struct pbr_material *mat, const char *filepath);

#endif  // ASSETS_H_
