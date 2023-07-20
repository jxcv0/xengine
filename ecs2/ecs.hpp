#ifndef ECS_HPP_
#define ECS_HPP_

#include "archetype.hpp"
#include "entity_mgr.hpp"

#include <stdexcept>

namespace xen
{

class ecs
{
public:
  eid_t create_entity();

  template <typename Component>
  void
  add_component(eid_t entity, const Component& val)
  {
    auto it = find_archetype_by_entity(entiy);
    if (it == m_archetypes.end())
      {
        throw std::runtime_error("Entity not found in archetypes");
      }

    if (!it->has_component<Component>())
      {
        auto cinfo = it->get_component_info();
        cinfo.push_back(
            { std::type_index(typeid(Component)), sizeof(Component) });
        archetype newarch(cinfo);
        newarch.add_entity(entity);
        newarch.get_component<Component>(entity) = val;

        /* Copy components over */
        for (std::size_t i = 0; i < cinfo.size() - 1; i++)
          {
            // newarch.get_component(entity, cinfo[i].index);
          }

        m_archetypes.push_back(newarch);
      }
    else
      {
        it->get_component<Component>() = val;
        return;
      }
  }

private:
  auto find_archetype_by_entity(eid_t entity);

private:
  entity_mgr m_entities;
  std::vector<archetype> m_archetypes;
};

} /* end of namespace xen */

#endif /* ECS_HPP_ */
