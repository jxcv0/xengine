#ifndef COMPONENTSUBSYSTEM_H_
#define COMPONENTSUBSYSTEM_H_

#include "componentarray.h"
#include "entitysubsystem.h"

class ComponentSubsystem {
 public:
 private:
  std::array<i_ComponentArray*, MAX_COMPONENTS> m_component_arrays;
};

#endif  // COMPONENTSUBSYSTEM_H_
