#include "ecs.hpp"
#include <algorithm>
#include <execution>

xen::eid_t
xen::ecs::create_entity()
{
  return m_entities.create_entity();
}