#ifndef ECS_HPP_
#define ECS_HPP_

#include "archetype.hpp"
#include "entity_mgr.hpp"

namespace xen
{

class ecs
{
public:
  ecs() = default;

  template <typename... ComponentTs>
  eid_t
  create_entity()
  {
    eid_t new_entity = m_entities.create_entity();
    return new_entity;
  }

private:
  entity_mgr m_entities;
  std::vector<archetype> m_archetypes;
};

} /* end of namespace xen */

#endif /* ECS_HPP_ */
