#include "pbr_deferred.h"

#include <stdio.h>
#include <stdlib.h>

#include "glad.h"
#include "utils.h"

#define MAX_NUM_LIGHTS 8

/**
 * ----------------------------------------------------------------------------
 */
static void load_pbrd_shaders(struct renderer *r) {
  char *v = load_file_into_mem("glsl/pbrd_geom.vert");
  char *c = load_file_into_mem("glsl/pbrd_geom.tesc");
  char *e = load_file_into_mem("glsl/pbrd_geom.tese");
  char *g = load_file_into_mem("glsl/pbrd_geom.geom");
  char *f = load_file_into_mem("glsl/pbrd_geom.frag");

  const char *vert_file = v;
  const char *tesc_file = c;
  const char *tese_file = e;
  const char *geom_file = g;
  const char *frag_file = f;

  GLuint vert_id = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert_id, 1, &vert_file, NULL);
  glCompileShader(vert_id);
  check_compile(vert_id);

  GLuint tesc_id = glCreateShader(GL_TESS_CONTROL_SHADER);
  glShaderSource(tesc_id, 1, &tesc_file, NULL);
  glCompileShader(tesc_id);
  check_compile(tesc_id);

  GLuint tese_id = glCreateShader(GL_TESS_EVALUATION_SHADER);
  glShaderSource(tese_id, 1, &tese_file, NULL);
  glCompileShader(tese_id);
  check_compile(tese_id);

  GLuint geom_id = glCreateShader(GL_GEOMETRY_SHADER);
  glShaderSource(geom_id, 1, &geom_file, NULL);
  glCompileShader(geom_id);
  check_compile(geom_id);

  GLuint frag_id = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag_id, 1, &frag_file, NULL);
  glCompileShader(frag_id);
  check_compile(frag_id);

  GLuint program_id = glCreateProgram();
  glAttachShader(program_id, vert_id);
  glAttachShader(program_id, tesc_id);
  glAttachShader(program_id, tese_id);
  glAttachShader(program_id, geom_id);
  glAttachShader(program_id, frag_id);

  glLinkProgram(program_id);
  check_link(program_id);

  glDeleteShader(vert_id);
  glDeleteShader(tesc_id);
  glDeleteShader(tese_id);
  glDeleteShader(geom_id);
  glDeleteShader(frag_id);

  free(v);
  free(c);
  free(e);
  free(g);
  free(f);

  r->deferred_geometry = program_id;
  r->deferred_lighting =
      load_shader_vf("glsl/pbrd_light.vert", "glsl/pbrd_light.frag");
}

/**
 * ----------------------------------------------------------------------------
 */
int pbrd_init(struct renderer *r, const uint32_t scr_w, const uint32_t scr_h) {
  load_pbrd_shaders(r);
  glPatchParameteri(GL_PATCH_VERTICES, 3);

  // set up G-Buffer
  glGenFramebuffers(1, &r->g_buff);
  glBindFramebuffer(GL_FRAMEBUFFER, r->g_buff);

  // vertex position buffer
  glGenTextures(1, &r->g_position);
  glBindTexture(GL_TEXTURE_2D, r->g_position);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, scr_w, scr_h, 0, GL_RGB, GL_FLOAT,
               NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         r->g_position, 0);

  // normal
  glGenTextures(1, &r->g_normal);
  glBindTexture(GL_TEXTURE_2D, r->g_normal);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, scr_w, scr_h, 0, GL_RGBA, GL_FLOAT,
               NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,
                         r->g_normal, 0);

  // diffuse
  glGenTextures(1, &r->g_tex_diff);
  glBindTexture(GL_TEXTURE_2D, r->g_tex_diff);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, scr_w, scr_h, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,
                         r->g_tex_diff, 0);

  const uint32_t attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                                   GL_COLOR_ATTACHMENT2};
  glDrawBuffers(3, attachments);

  // depth buffer
  glGenRenderbuffers(1, &r->g_depth);
  glBindRenderbuffer(GL_RENDERBUFFER, r->g_depth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, scr_w, scr_h);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, r->g_depth);

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
void pbrd_render_geometries(struct renderer *r, float projection[4][4],
                            float view[4][4], float *model_matrices[4][4],
                            struct geometry *geometries,
                            struct pbr_material *materials, size_t n) {
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindFramebuffer(GL_FRAMEBUFFER, r->g_buff);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(r->deferred_geometry);
  shader_set_uniform_m4fv(r->deferred_geometry, "projection", projection);
  shader_set_uniform_m4fv(r->deferred_geometry, "view", view);

  for (size_t i = 0; i < n; i++) {
    shader_set_uniform_m4fv(r->deferred_geometry, "model",
                            (float(*)[4])model_matrices[i]);
    shader_set_uniform_1i(r->deferred_geometry, "tex_diffuse", 0);
    shader_set_uniform_1i(r->deferred_geometry, "tex_roughness", 1);
    shader_set_uniform_1i(r->deferred_geometry, "tex_normal", 2);
    shader_set_uniform_1i(r->deferred_geometry, "tex_metallic", 3);
    // shader_set_uniform_1i(pbr.deferred_geometry, "tex_displacement", 4);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, materials[i].diffuse);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, materials[i].roughness);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, materials[i].normal);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, materials[i].metallic);
    // glActiveTexture(GL_TEXTURE4);
    // glBindTexture(GL_TEXTURE_2D, materials[i].displacement);

    glBindVertexArray(geometries[i].vao);

    glDrawElements(GL_PATCHES, geometries[i].num_indices, GL_UNSIGNED_INT, 0);
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void pbrd_render_lighting(struct renderer *r, struct light *lights,
                          size_t nlights, float view_pos[3], uint32_t scr_w,
                          uint32_t scr_h) {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(r->deferred_lighting);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, r->g_position);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, r->g_normal);

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, r->g_tex_diff);

  shader_set_uniform_1i(r->deferred_lighting, "g_position", 0);
  shader_set_uniform_1i(r->deferred_lighting, "g_normal", 1);
  shader_set_uniform_1i(r->deferred_lighting, "g_tex_diff", 2);

  shader_set_uniform_3fv(r->deferred_lighting, "view_pos", view_pos);
  shader_set_uniform_1ui(r->deferred_lighting, "num_lights", nlights);

  // TODO This can be made faster without the formatting
  char light[64];
  for (size_t i = 0; i < nlights; i++) {
    sprintf(light, "lights[%ld].m_position", i);
    shader_set_uniform_3fv(r->deferred_lighting, light, lights[i].position);
    sprintf(light, "lights[%ld].m_color", i);
    shader_set_uniform_3fv(r->deferred_lighting, light, lights[i].color);
    sprintf(light, "lights[%ld].m_constant", i);
    shader_set_uniform_1f(r->deferred_lighting, light, lights[i].constant);
    sprintf(light, "lights[%ld].m_linear", i);
    shader_set_uniform_1f(r->deferred_lighting, light, lights[i].linear);
    sprintf(light, "lights[%ld].m_quadratic", i);
    shader_set_uniform_1f(r->deferred_lighting, light, lights[i].quadratic);
  }

  // render quad
  glBindVertexArray(r->quad_vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindFramebuffer(GL_READ_FRAMEBUFFER, r->g_buff);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glBlitFramebuffer(0, 0, scr_w, scr_h, 0, 0, scr_w, scr_h, GL_DEPTH_BUFFER_BIT,
                    GL_NEAREST);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
