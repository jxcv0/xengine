#ifndef COORDINATOR_H_
#define COORDINATOR_H_

#include <vector>

#include "archetypearray.h"
#include "entityarray.h"

class Coordinator {
 public:
 private:
  EntityArray<128> m_entities;
  std::vector<ArchetypeArrayBase *> m_archetype_arrays;
};

#endif  // COORDINATOR_H_
