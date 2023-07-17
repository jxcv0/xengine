#include "entity_mgr.hpp"
#include <iostream>

xen::eid_t
xen::entity_mgr::create_entity()
{
  eid_t new_id;
  if (!m_freelist.empty())
    {
      new_id = m_freelist.back();
      m_freelist.pop_back();
    }
  else
    {
      new_id = m_counter++;
    }
  return new_id;
}

void
xen::entity_mgr::delete_entity(eid_t entity)
{
  m_freelist.push_back(entity);
}

std::size_t
xen::entity_mgr::num_entities()
{
  return m_counter - m_freelist.size();
}
