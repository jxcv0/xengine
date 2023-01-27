#include "render.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "glad.h"
#include "light.h"
#include "lin.h"
#include "mesh.h"
#include "shader.h"

// this must be the same as in the shader
#define MAX_NUM_LIGHTS 32

/**
 * ----------------------------------------------------------------------------
 */
int renderer_init(struct renderer *r, const uint32_t scr_w,
                  const uint32_t scr_h) {
  r->scr_w = scr_w;
  r->scr_h = scr_h;
  r->geom_shader = load_shader("render/glsl/deferred_geom.vert",
                               "render/glsl/deferred_geom.frag");
  r->light_shader = load_shader("render/glsl/deferred_light.vert",
                                "render/glsl/deferred_light.frag");
  // g-buffer
  glGenFramebuffers(1, &r->g_buff);
  glBindFramebuffer(GL_FRAMEBUFFER, r->g_buff);

  // positions
  glGenTextures(1, &r->g_pos);
  glBindTexture(GL_TEXTURE_2D, r->g_pos);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, scr_w, scr_h, 0, GL_RGBA, GL_FLOAT,
               NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         r->g_pos, 0);

  // normals
  glGenTextures(1, &r->g_norm);
  glBindTexture(GL_TEXTURE_2D, r->g_norm);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, scr_w, scr_h, 0, GL_RGBA, GL_FLOAT,
               NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,
                         r->g_norm, 0);

  // diffuse + specular textures
  glGenTextures(1, &r->g_diffspec);
  glBindTexture(GL_TEXTURE_2D, r->g_diffspec);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, scr_w, scr_h, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,
                         r->g_diffspec, 0);

  // TODO normal map

  uint32_t attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                             GL_COLOR_ATTACHMENT2};
  glDrawBuffers(3, attachments);

  // depth buffer
  glGenRenderbuffers(1, &r->depth_rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, r->depth_rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, scr_w, scr_h);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, r->depth_rbo);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    return -1;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // set up color attachments with g_buffer
  glUseProgram(r->light_shader);
  shader_set_uniform_1i(r->light_shader, "g_pos", 0);
  shader_set_uniform_1i(r->light_shader, "g_norm", 1);
  shader_set_uniform_1i(r->light_shader, "g_tex", 2);

  // quad buffers
  const float quad_verts[] = {
      -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
      1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
  };
  glGenVertexArrays(1, &r->quad_vao);
  glGenBuffers(1, &r->quad_vbo);
  glBindVertexArray(r->quad_vao);
  glBindBuffer(GL_ARRAY_BUFFER, r->quad_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quad_verts), quad_verts, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  return 0;
}

/**
 * ----------------------------------------------------------------------------
 */
void render_geometries(const struct renderer *r, const mat4 projection,
                       const mat4 view, const mat4 *models,
                       const struct mesh *meshes, const uint32_t n) {
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindFramebuffer(GL_FRAMEBUFFER, r->g_buff);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(r->geom_shader);
  shader_set_uniform_m4fv(r->geom_shader, "projection", projection);
  shader_set_uniform_m4fv(r->geom_shader, "view", view);

  // This assumes that vbo/vao have been generated elsewhere
  for (uint32_t i = 0; i < n; i++) {
    shader_set_uniform_m4fv(r->geom_shader, "model", models[i]);
    shader_set_uniform_1i(r->geom_shader, "tex_diff", 0);
    shader_set_uniform_1i(r->geom_shader, "tex_spec", 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, meshes[i].m_tex_diff);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, meshes[i].m_tex_spec);
    glBindVertexArray(meshes[i].m_vao);

    glDrawElements(GL_TRIANGLES, meshes[i].m_num_indices, GL_UNSIGNED_INT, 0);
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void render_lighting(struct renderer *r, struct light *lights, const uint32_t n,
                     const vec3 view_pos) {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(r->light_shader);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, r->g_pos);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, r->g_norm);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, r->g_diffspec);
  shader_set_uniform_1i(r->light_shader, "g_pos", 0);
  shader_set_uniform_1i(r->light_shader, "g_norm", 1);
  shader_set_uniform_1i(r->light_shader, "g_tex", 2);
  shader_set_uniform_3fv(r->light_shader, "view_pos", view_pos);
  shader_set_uniform_1ui(r->light_shader, "num_lights", n);

  // @perf This can be made faster with a custom formatter
  char light[32];
  for (uint32_t i = 0; i < n; i++) {
    sprintf(light, "lights[%d].m_position", i);
    shader_set_uniform_3fv(r->light_shader, light, lights[i].m_position);
    sprintf(light, "lights[%d].m_color", i);
    shader_set_uniform_3fv(r->light_shader, light, lights[i].m_color);
    sprintf(light, "lights[%d].m_constant", i);
    shader_set_uniform_1f(r->light_shader, light, lights[i].m_constant);
    sprintf(light, "lights[%d].m_linear", i);
    shader_set_uniform_1f(r->light_shader, light, lights[i].m_linear);
    sprintf(light, "lights[%d].m_quadratic", i);
    shader_set_uniform_1f(r->light_shader, light, lights[i].m_quadratic);
  }

  // render quad
  glBindVertexArray(r->quad_vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindFramebuffer(GL_READ_FRAMEBUFFER, r->g_buff);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glBlitFramebuffer(0, 0, r->scr_w, r->scr_h, 0, 0, r->scr_w, r->scr_h,
                    GL_DEPTH_BUFFER_BIT, GL_NEAREST);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
