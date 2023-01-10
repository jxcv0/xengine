#include "deferred_render.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "glad.h"
#include "shader.h"

unsigned int g_buffer;
unsigned int rbo_depth;
unsigned int g_pos;
unsigned int g_norm;
unsigned int g_tex;

shader_t geom_shader;

// TODO ubo for projection/view

void dr_init(const uint32_t scr_w, const uint32_t scr_h) {
  glGenFramebuffers(1, &g_buffer);
  glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);

  // position color buffer
  glGenTextures(1, &g_pos);
  glBindTexture(GL_TEXTURE_2D, g_pos);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, scr_w, scr_h, 0, GL_RGBA, GL_FLOAT,
               NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         g_pos, 0);

  // normal color buffer
  glGenTextures(1, &g_norm);
  glBindTexture(GL_TEXTURE_2D, g_pos);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, scr_w, scr_h, 0, GL_RGBA, GL_FLOAT,
               NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,
                         g_norm, 0);

  // diffuse and specular color buffer
  glGenTextures(1, &g_tex);
  glBindTexture(GL_TEXTURE_2D, g_tex);
  glTexImage2D(GL_TEXTURE, 0, GL_RGBA, scr_w, scr_h, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,
                         g_tex, 0);

  // tell opengl about separate render targets
  const unsigned int attachments[3] = {
      GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
  glDrawBuffers(3, attachments);

  glGenRenderbuffers(1, &rbo_depth);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, scr_w, scr_h);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, rbo_depth);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    printf("framebuffer incomplete\n");
    exit(EXIT_FAILURE);
  }

  geom_shader = shader_load("render/glsl/deferred_geom.vert",
                       "render/glsl/deferred_geom.frag");
}

