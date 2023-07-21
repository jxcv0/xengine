#ifndef ECS_HPP_
#define ECS_HPP_

#include <algorithm>
#include <cstring>
#include <stdexcept>

#include "archetype.hpp"
#include "entity_mgr.hpp"

namespace xen {

class ecs {
 public:

 private:
  entity_mgr m_entities;
  std::vector<archetype> m_archetypes;
};

} /* end of namespace xen */

#endif /* ECS_HPP_ */
