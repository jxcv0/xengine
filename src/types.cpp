#include "types.h"

// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::~Texture() { stbi_image_free(mp_data); }
