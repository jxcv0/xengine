#ifndef TEXT_H_
#define TEXT_H_

#include <stddef.h>

#include "lin.h"
#include "shader.h"

void init_ttf(char *filepath);

void render_text(GLuint shader, float projection[4][4], float xpos, float ypos,
                 float color[4], char *txt, size_t n);

#endif  // TEXT_H_
