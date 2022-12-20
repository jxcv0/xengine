#ifndef ECS_H_
#define ECS_H_

#include "componentarray.h"
#include "entityarray.h"
#include "mat4.h"
#include "mesh.h"
#include "shader.h"
#include "threadpool.h"

class ECS {
 public:
  ECS() : m_threadpool(4) {}

 private:
  EntityArray m_entities;
  ComponentArray<Mesh> m_meshes;
  ComponentArray<Mat4> m_translations;  // model matrices
  ComponentArray<Shader> m_shaders;
  ThreadPool m_threadpool;
};

#endif  // ECS_H_
