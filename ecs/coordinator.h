#ifndef COORDINATOR_H_
#define COORDINATOR_H_

#include "component.h"
#include "entity.h"
#include "threadpool.h"

class Coordinator {
 public:
  Coordinator() : m_threadpool(4) {}
  ~Coordinator() = default;

 private:
  ThreadPool m_threadpool;
  EntitySystem<> m_entity_system;
  std::vector<ComponentArrayBase*> m_component_arrays;
};

#endif  // COORDINATOR_H_
