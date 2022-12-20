#ifndef ECS_H_
#define ECS_H_

#include <cstring>

#include "componentarray.h"
#include "constants.h"
#include "entityarray.h"
#include "mat4.h"
#include "mesh.h"
#include "shader.h"
#include "threadpool.h"

/**
 * @brief Entity Component System (system).
 */
class ECS {
 public:
  ECS() : m_threadpool(4), m_num_systems(0) {}

  int register_system(Task *system) {
    if (m_num_systems != MAX_SYSTEMS) {
      m_systems[m_num_systems++] = system;
      return 0;
    }
    return -1;
  }

  /**
   * @brief Schedule all tasks then wait for completion.
   */
  void update() {
    for (int i = 0; i < m_num_systems; i++) {
      m_threadpool.schedule_task(m_systems[i]);
    }
    m_threadpool.wait();
  }

 private:
  EntityArray m_entities;
  ComponentArray<Mesh> m_meshes;
  ComponentArray<Mat4> m_translations;  // model matrices
  ComponentArray<Shader> m_shaders;
  ThreadPool m_threadpool;
  int m_num_systems;
  Task *m_systems[MAX_SYSTEMS];
};

#endif  // ECS_H_
