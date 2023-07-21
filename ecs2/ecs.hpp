#ifndef ECS_HPP_
#define ECS_HPP_

#include "archetype.hpp"
#include "entity_mgr.hpp"
#include <algorithm>
#include <cstring>
#include <stdexcept>

namespace xen
{

class ecs
{
public:
  eid_t create_entity();

  eid_t
  num_entities() const
  {
    return m_entities.num_entities();
  }

  eid_t
  num_archetypes() const
  {
    return m_archetypes.size();
  }

  template <typename Component>
  Component&
  get_component(eid_t entity)
  {
    if (!m_entities.is_valid_id(entity))
      {
        throw std::runtime_error("Invalid entity ID");
      }

    auto it = find_archetype_by_entity(entity);
    if (it == m_archetypes.end())
      {
        /* create new archetype and add entity */
        xen::archetype arch(
            { { std::type_index(typeid(Component)), sizeof(Component) } });
        arch.add_entity(entity);
        m_archetypes.push_back(arch);
        return arch.get_component<Component>(entity);
      }
    return *(new Component);
  }

private:
  auto
  find_archetype_by_entity(eid_t entity)
  {
    return std::find_if(
        m_archetypes.begin(), m_archetypes.end(),
        [entity](const auto& arch) { return arch.has_entity(entity); });
  }

private:
  entity_mgr m_entities;
  std::vector<archetype> m_archetypes;
};

} /* end of namespace xen */

#endif /* ECS_HPP_ */
