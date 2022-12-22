#ifndef ECS_H_
#define ECS_H_

#include "componentarray.h"
#include "constants.h"
#include "entityarray.h"
#include "mesh.h"
#include "threadpool.h"

/**
 * @brief Entity Component System (system).
 */
class ECS {
 public:
  ECS()
      : m_threadpool(4),
        m_num_systems(0)

            entity_id create_entity() {
    return m_entities.create();
  }

  int create_mesh(const entity_id id, const char *filepath) {
    (void)filepath;
    if (m_mesh_array.assign(id) == 0) {
    }
  }

  void update() {
    for (int i = 0; i < m_num_systems; i++) {
      m_threadpool.schedule_task(m_systems[i]);
    }
    m_threadpool.wait();  // TODO a smarter way of coordinating
  }

 private:
  EntityArray m_entities;

  ThreadPool m_threadpool;
  int m_num_systems;
  Task *m_systems[MAX_SYSTEMS];

  // mesh component
  ComponentArray<Mesh> m_mesh_array;
};

// mesh
// translation
// shader

#endif  // ECS_H_
