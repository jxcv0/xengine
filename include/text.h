#ifndef TEXT_H_
#define TEXT_H_

#include <stddef.h>

#include "lin.h"
#include "shader.h"

void init_ttf(char *filepath);

void render_text(GLuint shader, mat4 projection, vec2 position, vec4 color,
                 char *txt, size_t n);

#endif  // TEXT_H_
