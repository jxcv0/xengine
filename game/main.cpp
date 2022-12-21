#include <math.h>

#include <iostream>
#include <type_traits>

#include "camera.h"
#include "checkerr.h"
#include "componentarray.h"
#include "entityarray.h"
#include "lin.h"
#include "mat4.h"
#include "mesh.h"
#include "shader.h"
#include "vec2.h"
#include "vec3.h"
#include "window.h"

constexpr auto window_width = 1080;
constexpr auto window_height = 600;

Camera camera(Vec3(0, 0, 3), window_width / 2.0f, window_height / 2.0f);

EntityArray entities;
ComponentArray<Mesh> mesh_components;
ComponentArray<Vec3> translation_components;

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {
  Window window(window_width, window_height, "xengine");

  auto shader =
      ShaderUtils::load("render/glsl/uber.vert", "render/glsl/uber.frag");

  auto projection_matrix = window.projection_matrix(60);

  // entity
  entity_id id = entities.create();
  mesh_components.assign(id);
  translation_components.assign(id);

  translation_components.set(id, Vec3(0, 0, 0));

  Mat4 model_matrix = lin::translate(Mat4(1), *translation_components.get(id));
  model_matrix = lin::rotate(model_matrix, Vec3(1, 0.8, 0), lin::radians(-55));

  mesh_components.get(id)->load("assets/models/cyborg/cyborg.obj");
  mesh_components.get(id)->gen_buffers();

  shader.use();
  shader.set_uniform("projection", &projection_matrix);

  Vec3 light_pos(1, 1, 1);
  Vec3 light_color(1, 1, 1);
  Vec3 object_color(1, 0.5, 0.31);
  Vec2 mouse_pos;

  while (!window.should_close()) {
    mouse_pos = window.cursor_position();
    camera.process_mouse_movement(&mouse_pos);
    window.clear_buffers();
    glClearColor(0.2, 0.3, 0.3, 1);

    // transforms
    shader.set_uniform("model", &model_matrix);
    Mat4 view_matrix = camera.view_matrix();
    shader.set_uniform("view", &view_matrix);

    // lighting
    shader.set_uniform("light_pos", &light_pos);
    shader.set_uniform("light_color", &light_color);
    shader.set_uniform("obj_color", &object_color);

    mesh_components.get(id)->draw();

    window.swap_buffers();
    window.poll_events();
  }

  return 0;
}