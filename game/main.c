#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "shader.h"
#include "text.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "camera.h"
#include "glad.h"
#include "input.h"
#include "lin.h"
#include "mesh.h"
#include "render.h"
#include "window.h"

#define MAX_MESHES 64

extern const vec3 GLOBAL_UP;

void print_mat4(const char *name, const mat4 m) {
  printf("mat4: %s {\n", name);
  for (int i = 0; i < 4; i++) {
    printf("%f, %f, %f, %f\n", m[i][0], m[i][1], m[i][2], m[i][3]);
  }
  printf("}\n");
}

GLFWwindow *window;
const float window_width = 1080;
const float window_height = 600;

struct camera camera = {.m_mouse_sensetivity = 0.3, .m_movement_speed = 0.1};
vec2 mouse_pos;
mat4 projection_matrix = {0};
mat4 view_matrix = {0};

void update_view_matrix() {
  vec3 ctr = {camera.m_pos[0] + camera.m_view_dir[0],
              camera.m_pos[1] + camera.m_view_dir[1],
              camera.m_pos[2] + camera.m_view_dir[2]};

  look_at(view_matrix, camera.m_pos, ctr, camera.m_up);
}

void handle_mouse_movement(GLFWwindow *w, double x, double y) {
  (void)w;
  mouse_pos[0] = (float)x;
  mouse_pos[1] = (float)y;
  process_mouse_movement(&camera, mouse_pos);
}

// move the fps camera with wasd and update mouse input
void handle_keyboard_input(GLFWwindow *w) {
  if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) {
    vec3 forward;
    cross_vec3(forward, GLOBAL_UP, camera.m_right);
    camera.m_pos[0] += forward[0] * camera.m_movement_speed;
    camera.m_pos[1] += forward[1] * camera.m_movement_speed;
    camera.m_pos[2] += forward[2] * camera.m_movement_speed;
  }

  if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) {
    vec3 forward;
    cross_vec3(forward, GLOBAL_UP, camera.m_right);
    camera.m_pos[0] -= forward[0] * camera.m_movement_speed;
    camera.m_pos[1] -= forward[1] * camera.m_movement_speed;
    camera.m_pos[2] -= forward[2] * camera.m_movement_speed;
  }

  if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) {
    camera.m_pos[0] -= camera.m_right[0] * camera.m_movement_speed;
    camera.m_pos[1] -= camera.m_right[1] * camera.m_movement_speed;
    camera.m_pos[2] -= camera.m_right[2] * camera.m_movement_speed;
  }

  if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) {
    camera.m_pos[0] += camera.m_right[0] * camera.m_movement_speed;
    camera.m_pos[1] += camera.m_right[1] * camera.m_movement_speed;
    camera.m_pos[2] += camera.m_right[2] * camera.m_movement_speed;
  }
}

void render_quad(void);

// main
int main(int argc, char const *argv[]) {
  (void)argc;
  (void)argv;

  create_window(&window, window_width, window_height, "game");
  glfwSetCursorPosCallback(window, handle_mouse_movement);

  perspective(projection_matrix, radians(60),
              ((float)window_width / (float)window_height), 0.1f, 100.0f);

  struct mesh test_mesh = load_mesh("assets/models/cyborg/cyborg.obj");
  struct mesh floor = load_mesh("assets/models/floor/floor.obj");

  gen_mesh_buffers(&test_mesh);
  gen_mesh_buffers(&floor);

  struct light light = {0};
  light.m_color[0] = 1;
  light.m_color[1] = 1;
  light.m_color[2] = 1;
  light.m_position[0] = 0;
  light.m_position[1] = 2;
  light.m_position[2] = 0;
  light.m_constant = 1.0;
  light.m_linear = 0.0014;
  light.m_quadratic = 0.000007;

  camera.m_pos[0] = 0;
  camera.m_pos[1] = 1.86;
  camera.m_pos[2] = 3;

  camera.m_last_mouse_pos[1] = window_height / 2.0f;
  camera.m_yaw = 275;
  process_mouse_movement(&camera, mouse_pos);

  shader_t geom_shader = load_shader("render/glsl/deferred_geom.vert",
                                     "render/glsl/deferred_geom.frag");
  shader_t light_shader = load_shader("render/glsl/deferred_light.vert",
                                      "render/glsl/deferred_light.frag");

  unsigned int gBuffer;
  glGenFramebuffers(1, &gBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
  unsigned int gPosition, gNormal, gAlbedoSpec;
  // position color buffer
  glGenTextures(1, &gPosition);
  glBindTexture(GL_TEXTURE_2D, gPosition);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window_width, window_height, 0,
               GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         gPosition, 0);
  // normal color buffer
  glGenTextures(1, &gNormal);
  glBindTexture(GL_TEXTURE_2D, gNormal);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window_width, window_height, 0,
               GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,
                         gNormal, 0);
  // color + specular color buffer
  glGenTextures(1, &gAlbedoSpec);
  glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window_width, window_height, 0,
               GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,
                         gAlbedoSpec, 0);
  // tell OpenGL which color attachments we'll use (of this framebuffer) for
  // rendering
  unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                                 GL_COLOR_ATTACHMENT2};
  glDrawBuffers(3, attachments);
  // create and attach depth buffer (renderbuffer)
  unsigned int rboDepth;
  glGenRenderbuffers(1, &rboDepth);
  glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window_width,
                        window_height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, rboDepth);
  // finally check if framebuffer is complete
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    printf("Framebuffer incomplete. Exiting\n");
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glUseProgram(light_shader);
  shader_set_uniform_1i(light_shader, "g_pos", 0);
  shader_set_uniform_1i(light_shader, "g_norm", 1);
  shader_set_uniform_1i(light_shader, "g_tex", 2);

  while (!glfwWindowShouldClose(window)) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }

    handle_keyboard_input(window);
    update_view_matrix();

    mat4 model_matrix;
    identity_mat4(model_matrix);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glad_glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(geom_shader);
    shader_set_uniform_m4fv(geom_shader, "projection", projection_matrix);
    shader_set_uniform_m4fv(geom_shader, "view", view_matrix);
    shader_set_uniform_m4fv(geom_shader, "model", model_matrix);
    shader_set_uniform_1i(geom_shader, "tex_diff", 0);
    shader_set_uniform_1i(geom_shader, "tex_spec", 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,
                  test_mesh.m_material.m_tex_diffuse.m_texture_id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,
                  test_mesh.m_material.m_tex_specular.m_texture_id);
    glBindVertexArray(test_mesh.m_vao);
    glDrawArrays(GL_TRIANGLES, 0, test_mesh.m_num_vertices);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(light_shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    shader_set_uniform_1i(light_shader, "g_pos", 0);
    shader_set_uniform_1i(light_shader, "g_norm", 1);
    shader_set_uniform_1i(light_shader, "g_tex", 2);
    shader_set_uniform_3fv(light_shader, "light[0].m_position",
                           light.m_position);
    shader_set_uniform_3fv(light_shader, "light[0].m_color", light.m_color);
    shader_set_uniform_1f(light_shader, "light[0].m_constant",
                          light.m_constant);
    shader_set_uniform_1f(light_shader, "light[0].m_linear", light.m_linear);
    shader_set_uniform_1f(light_shader, "light[0].m_quadratic",
                          light.m_quadratic);
    shader_set_uniform_3fv(light_shader, "view_pos", camera.m_pos);
    render_quad();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, window_width, window_height, 0, 0, window_width, window_height, GL_DEPTH_BUFFER_BIT, GL_NEAREST); 
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  unload_mesh(&test_mesh);
  unload_mesh(&floor);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void render_quad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
