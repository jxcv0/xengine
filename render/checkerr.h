#ifndef CHECKERR_H_
#define CHECKERR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <glad.h>

void APIENTRY gl_debug_output(GLenum source, GLenum type, unsigned int id,
                              GLenum severity, GLsizei length,
                              const char *message, const void *userParam);

#ifdef __cplusplus
}
#endif

#endif  // CHECKERR_H_
