#include "pbr_deferred.h"

#include <stdio.h>
#include <stdlib.h>

#include "glad.h"
#include "shader.h"

// we want this all in the same block of memory.
static struct {
  // geometry
  uint32_t g_buff;
  uint32_t g_pos;

  // normal matrix
  uint32_t g_tangent;
  uint32_t g_bitangent;
  uint32_t g_normal;

  // textures
  uint32_t g_tex_diff;
  uint32_t g_tex_norm;

  // depth
  uint32_t g_depth;

  // lighting
  uint32_t quad_vbo;
  uint32_t quad_vao;

  shader_t deferred_geometry;
  shader_t deferred_lighting;
} pbr;

/**
 * ----------------------------------------------------------------------------
 */
int pbrd_init(const uint32_t scr_w, const uint32_t scr_h) {
  pbr.deferred_geometry =
      load_shader("glsl/pbrd_geom.vert", "glsl/pbrd_geom.frag");
  pbr.deferred_lighting =
      load_shader("glsl/pbrd_light.vert", "glsl/pbrd_light.frag");

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

  // tangent
  glGenTextures(1, &pbr.g_tangent);
  glBindTexture(GL_TEXTURE_2D, pbr.g_tangent);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, scr_w, scr_h, 0, GL_RGBA, GL_FLOAT,
               NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,
                         pbr.g_tangent, 0);

  // bitangent
  glGenTextures(1, &pbr.g_bitangent);
  glBindTexture(GL_TEXTURE_2D, pbr.g_bitangent);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, scr_w, scr_h, 0, GL_RGB, GL_FLOAT,
               NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,
                         pbr.g_bitangent, 0);

  // normal
  glGenTextures(1, &pbr.g_normal);
  glBindTexture(GL_TEXTURE_2D, pbr.g_normal);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, scr_w, scr_h, 0, GL_RGB, GL_FLOAT,
               NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D,
                         pbr.g_normal, 0);

  // diffuse + roughness texture
  glGenTextures(1, &pbr.g_tex_diff);
  glBindTexture(GL_TEXTURE_2D, pbr.g_tex_diff);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, scr_w, scr_h, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D,
                         pbr.g_tex_diff, 0);

  // normal texture
  glGenTextures(1, &pbr.g_tex_norm);
  glBindTexture(GL_TEXTURE_2D, pbr.g_tex_norm);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, scr_w, scr_h, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D,
                         pbr.g_tex_diff, 0);

  const uint32_t attachments[6] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                                   GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
                                   GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5};
  glDrawBuffers(6, attachments);

  // depth buffer
  glGenRenderbuffers(1, &pbr.g_depth);
  glBindRenderbuffer(GL_RENDERBUFFER, pbr.g_depth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, scr_w, scr_h);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, pbr.g_depth);

  glUseProgram(pbr.deferred_lighting);
  shader_set_uniform_1i(pbr.deferred_lighting, "g_pos", 0);
  shader_set_uniform_1i(pbr.deferred_lighting, "g_norm", 1);
  shader_set_uniform_1i(pbr.deferred_lighting, "g_tex_diff", 2);
  shader_set_uniform_1i(pbr.deferred_lighting, "g_tex_norm", 3);

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

/**
 * ----------------------------------------------------------------------------
 */
void pbrd_render_geometries(const mat4 projection, const mat4 view,
                            const vec3 *positions, const struct geometry *geoms,
                            const struct pbr_material *mats, const uint32_t n) {
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindFramebuffer(GL_FRAMEBUFFER, pbr.g_buff);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(pbr.deferred_geometry);
  shader_set_uniform_m4fv(pbr.deferred_geometry, "projection", projection);
  shader_set_uniform_m4fv(pbr.deferred_geometry, "view", view);

  // TODO this should be a component as it can be parallelized.
  mat4 model = {0};
  for (uint32_t i = 0; i < n; i++) {
    identity_mat4(model);            // see above
    translate(model, positions[i]);  // see above
    shader_set_uniform_m4fv(pbr.deferred_geometry, "model", model);
    shader_set_uniform_1i(pbr.deferred_geometry, "tex_diffuse", 0);
    shader_set_uniform_1i(pbr.deferred_geometry, "tex_roughness", 1);
    shader_set_uniform_1i(pbr.deferred_geometry, "tex_normal", 2);
    shader_set_uniform_1i(pbr.deferred_geometry, "tex_displacement", 3);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mats[i].m_diffuse);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mats[i].m_roughness);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mats[i].m_normal);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, mats[i].m_displacement);

    glBindVertexArray(geoms[i].m_vao);

    glDrawElements(GL_TRIANGLES, geoms[i].m_num_indices, GL_UNSIGNED_INT, 0);
  }
}

void pbrd_render_lighting(struct light *lights, const uint32_t n,
                          const vec3 view_pos, const uint32_t scr_w,
                          const uint32_t scr_h) {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(pbr.deferred_lighting);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, pbr.g_pos);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, pbr.g_normal);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, pbr.g_tex_diff);

  shader_set_uniform_1i(pbr.deferred_lighting, "g_pos", 0);

  shader_set_uniform_1i(pbr.deferred_lighting, "g_tangent", 1);
  shader_set_uniform_1i(pbr.deferred_lighting, "g_bitangent", 2);
  shader_set_uniform_1i(pbr.deferred_lighting, "g_normal", 3);

  shader_set_uniform_1i(pbr.deferred_lighting, "g_tex_diff", 4);
  shader_set_uniform_1i(pbr.deferred_lighting, "g_tex_norm", 5);

  shader_set_uniform_3fv(pbr.deferred_lighting, "view_pos", view_pos);
  shader_set_uniform_1ui(pbr.deferred_lighting, "num_lights", n);

  // TODO This can be made faster without the formatting
  char light[32];
  for (uint32_t i = 0; i < n; i++) {
    sprintf(light, "lights[%d].m_position", i);
    shader_set_uniform_3fv(pbr.deferred_lighting, light, lights[i].m_position);
    sprintf(light, "lights[%d].m_color", i);
    shader_set_uniform_3fv(pbr.deferred_lighting, light, lights[i].m_color);
    sprintf(light, "lights[%d].m_constant", i);
    shader_set_uniform_1f(pbr.deferred_lighting, light, lights[i].m_constant);
    sprintf(light, "lights[%d].m_linear", i);
    shader_set_uniform_1f(pbr.deferred_lighting, light, lights[i].m_linear);
    sprintf(light, "lights[%d].m_quadratic", i);
    shader_set_uniform_1f(pbr.deferred_lighting, light, lights[i].m_quadratic);
  }

  // render quad
  glBindVertexArray(pbr.quad_vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindFramebuffer(GL_READ_FRAMEBUFFER, pbr.g_buff);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glBlitFramebuffer(0, 0, scr_w, scr_h, 0, 0, scr_w, scr_h, GL_DEPTH_BUFFER_BIT,
                    GL_NEAREST);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
