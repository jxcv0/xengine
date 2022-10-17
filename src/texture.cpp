#include "texture.h"

#include "stb_image.h"

Texture::~Texture() { stbi_image_free(mp_data); }
