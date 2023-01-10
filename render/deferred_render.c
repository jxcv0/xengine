#include "deferred_render.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "glad.h"
#include "lin.h"
#include "mesh.h"
#include "shader.h"

static unsigned int g_buffer;
static unsigned int rbo_depth;
static unsigned int g_pos;
static unsigned int g_norm;
static unsigned int g_tex;

static shader_t geom_shader;
static shader_t lighting_shader;

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

  lighting_shader = shader_load("render/glsl/deferred_light.vert",
                                "render/glsl/deferred_light.frag");
}

/**
 * ----------------------------------------------------------------------------
 */
void dr_geometry_pass(const mat4 projection, const mat4 view,
                      const mat4 *model_matrices, const struct mesh *meshes,
                      const uint32_t n, const vec3 view_pos) {
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);

  glUseProgram(geom_shader);

  // transforms
  shader_set_uniform_m4fv(geom_shader, "projection", projection);
  shader_set_uniform_m4fv(geom_shader, "view", view);

  // camera position
  shader_set_uniform_3fv(geom_shader, "view_pos", view_pos);

  for (uint32_t i = 0; i < n; i++) {
    shader_set_uniform_m4fv(geom_shader, "model", model_matrices[i]);

    // textures
    // TODO normal mapping possible?
    shader_set_uniform_1i(geom_shader, "tex_diff", 0);
    shader_set_uniform_1i(geom_shader, "tex_spec", 1);
    // shader_set_uniform_1i(geom_shader, "tex_norm", 2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,
                  meshes[i].m_material.m_tex_diffuse.m_texture_id);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,
                  meshes[i].m_material.m_tex_specular.m_texture_id);

    /*
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,
                  meshes[i].m_material.m_tex_normal.m_texture_id);
    */

    glBindVertexArray(meshes[i].m_vao);
    glDrawArrays(GL_TRIANGLES, 0, meshes[i].m_num_vertices);
  }
}
