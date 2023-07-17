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

  /**
   * @brief Create an entity with components. A new archetype of the component
   * types is created if one does not already exist.
   *
   * @tparam ComponentTs The component types
   * @return eid_t
   */
  template <typename... ComponentTs>
  eid_t
  create_entity()
  {
    return m_entities.create_entity();
  }

  /**
   * @brief Get the number of archetypes in the ECS.
   *
   * @return std::size_t
   */
  std::size_t
  num_archetypes()
  {
    return m_archetypes.size();
  }

  template <typename Component>
  Component&
  get(eid_t entity)
  {
    auto it = std::find_if(m_archetypes.begin(), m_archetypes.end(),
                           [=](const auto& a) { a.has_entity(entity); });

    /* Entity must have no components yet */
    if (it == m_archetypes.end())
      {
        archetype new_arch = archetype::create<Component>();
        new_arch.add_entity(entity);
        m_archetypes.push_back(new_arch);
        return new_arch.get<Component>(entity);
      }

    if (it->template has_types<Component>())
      {
        return it.template get<Component>(entity);
      }
    else
      {
        /* Move entity out of archetype into new one */
      }

    /* Entity is already part of an archetype */
    auto arch = *it;
    return arch.template get<Component>(entity);
  }

private:
  entity_mgr m_entities;
  std::vector<archetype> m_archetypes;
};

} /* end of namespace xen */

#endif /* ECS_HPP_ */
