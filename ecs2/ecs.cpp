#include "ecs.hpp"
#include <algorithm>
#include <execution>

xen::eid_t
xen::ecs::create_entity()
{
  return m_entities.create_entity();
}

auto
xen::ecs::find_archetype_by_entity(eid_t entity)
{
  return std::find_if(m_archetypes.begin(), m_archetypes.end(),
      [entity](const auto& arch) { return arch.has_entity(entity); });
}