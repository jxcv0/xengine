#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include "mat4.h"
#include "mesh.h"
#include "shader.h"
#include "vec3.h"

struct MeshComponent {
  const static int component_id = (1 << 1);
  Mesh m_mesh;
};

struct TransformationComponent {
  const static int component_id = (1 << 2);
  Vec3 m_position;
  Mat4 m_rotation;
};

struct ModelMatrixComponent {
  const static int component_id = (1 << 3);
  Mat4 m_model_matrix;
};

struct ShaderComponent {
  const static int component_id = (1 << 4);
  Shader m_shader;
};

#endif  // COMPONENTS_H_
