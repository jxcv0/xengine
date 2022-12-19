#ifndef ECS_H_
#define ECS_H_

#include <concepts>
#include <vector>

#include "archetypearray.h"
#include "entityarray.h"
#include "threadpool.h"

class ECS {
 public:
 private:
  EntityArray<128> m_entities;
  std::vector<ArchetypeArrayBase *> m_archetype_arrays;
  std::vector<Task *> m_tasks;
};

#endif  // ECS_H_
