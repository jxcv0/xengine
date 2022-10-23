#include "texture.h"

Texture::~Texture() {
  if (mp_data != nullptr) {
    stbi_image_free(mp_data);
  }
}
