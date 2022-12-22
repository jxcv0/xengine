#ifndef ECS_H_
#define ECS_H_

#include "componentarray.h"
#include "constants.h"
#include "entityarray.h"
#include "threadpool.h"

/**
 * @brief Entity Component System (system).
 */
class ECS {
 public:
  ECS() : m_threadpool(4), m_num_systems(0), m_num_component_arrays(0) {}

  entity_id create_entity() { return m_entities.create(); }

  template <typename ComponentType>
  int register_component(ComponentArrayBase *arr) {
    if (m_num_component_arrays != MAX_COMPONENTS) {
      m_component_arrays[m_num_component_arrays++] = arr;
      return 0;
    }
    return -1;
  }

  // int register_system(Task *system) { /* TODO */ }

  void update() {
    for (int i = 0; i < m_num_systems; i++) {
      m_threadpool.schedule_task(m_systems[i]);
    }
    m_threadpool.wait();  // TODO a smarter way of coordinating
  }

  template <typename ComponentType>
  ComponentArray<ComponentType> *get_component_array() {
    for (int i = 0; i < m_num_component_arrays; i++) {
      ComponentArrayBase *c = m_component_arrays[m_num_component_arrays];
      if (c->component_id() == ComponentType::component_id) {
        return static_cast<ComponentArray<ComponentType> *>(c);
      }
    }
    return nullptr;
  }

 private:
  EntityArray m_entities;

  ThreadPool m_threadpool;
  int m_num_systems;
  Task *m_systems[MAX_SYSTEMS];

  // pointers to component arrays
  int m_num_component_arrays;
  ComponentArrayBase *m_component_arrays[MAX_COMPONENTS];
};

// mesh
// translation
// shader

#endif  // ECS_H_
