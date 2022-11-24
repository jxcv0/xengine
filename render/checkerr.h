#ifndef CHECKERR_H_
#define CHECKERR_H_

#include <glad.h>

#include <iostream>

int gl_print_error(std::ostream &os, const char *file, int line);

void APIENTRY gl_debug_output(GLenum source, GLenum type, unsigned int id,
                              GLenum severity, GLsizei length,
                              const char *message, const void *userParam);

#endif  // CHECKERR_H_
