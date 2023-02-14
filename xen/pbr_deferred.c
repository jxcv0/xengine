#include "pbr_deferred.h"

#include "assets.h"
#include "glad.h"
#include <stdlib.h>
#include <stdio.h>

// we want this all in the same block of memory.
static struct {
  uint32_t g_buff;
  uint32_t g_pos;
  uint32_t g_norm;
  uint32_t g_depth;
  uint32_t g_tex_diff;
  uint32_t g_tex_norm;
  uint32_t quad_vbo;
  uint32_t quad_vao;
} pbr;

/**
 * ----------------------------------------------------------------------------
 */
int pbrd_init(const uint32_t scr_w, const uint32_t scr_h) {
  // TODO do textures need to be rgba?
  // TODO load shaders

  // set up G-Buffer
  glGenFramebuffers(1, &pbr.g_buff);
  glBindFramebuffer(GL_FRAMEBUFFER, pbr.g_buff);

  // vertex position buffer
  glGenTextures(1, &pbr.g_pos);
  glBindTexture(GL_TEXTURE_2D, pbr.g_pos);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, scr_w, scr_h, 0, GL_RGBA, GL_FLOAT,
               NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         pbr.g_pos, 0);

  // vertex normal buffer
  glGenTextures(1, &pbr.g_norm);
  glBindTexture(GL_TEXTURE_2D, pbr.g_norm);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, scr_w, scr_h, 0, GL_RGBA, GL_FLOAT,
               NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,
                         pbr.g_norm, 0);

  // diffuse texture buffer
  // roughness stored in alpha component
  glGenTextures(1, &pbr.g_tex_diff);
  glBindTexture(GL_TEXTURE_2D, pbr.g_tex_diff);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, scr_w, scr_h, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,
                         pbr.g_tex_diff, 0);

  // normal texture buffer
  // displacement stored in alpha component
  glGenTextures(1, &pbr.g_tex_norm);
  glBindTexture(GL_TEXTURE_2D, pbr.g_tex_norm);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, scr_w, scr_h, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D,
                         pbr.g_tex_diff, 0);

  const uint32_t attachments[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                                   GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT4};
  glDrawBuffers(4, attachments);

  // depth buffer
  glGenRenderbuffers(1, &pbr.g_depth);
  glBindRenderbuffer(GL_RENDERBUFFER, pbr.g_depth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, scr_w, scr_h);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, pbr.g_depth);

  // glUseProgram(pbr.deferred_lighting)
  // shader_set_uniform_1i(pbr.deferred_lighting, "g_pos", 0);
  // shader_set_uniform_1i(pbr.deferred_lighting, "g_norm", 1);
  // shader_set_uniform_1i(pbr.deferred_lighting, "g_tex_diff", 2);
  // shader_set_uniform_1i(pbr.deferred_lighting, "g_tex_norm", 3);

  const float quad_verts[] = {
      -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
      1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
  };

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    fprintf(stderr, "Framebuffer incomplete. Exiting ...");
    exit(EXIT_FAILURE);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // set up lighting shader
  glGenVertexArrays(1, &pbr.quad_vao);
  glGenBuffers(1, &pbr.quad_vbo);
  glBindVertexArray(pbr.quad_vao);
  glBindBuffer(GL_ARRAY_BUFFER, pbr.quad_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quad_verts), quad_verts, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  return 0;
}
