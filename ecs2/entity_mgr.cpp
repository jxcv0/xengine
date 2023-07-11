#include "entity_mgr.h"

std::uint64_t
xen::entity_mgr::create_entity ()
{
  std::uint64_t new_id;
  if (!m_freelist.empty ())
    {
      new_id = m_freelist.back ();
      m_freelist.pop_back ();
    }
  else
    {
      new_id = m_counter++;
    }
  m_map[new_id] = std::vector<std::size_t> ();
  return new_id;
}

void
xen::entity_mgr::delete_entity (std::uint64_t entity)
{
  m_map.erase (entity);
  --m_counter;
}

std::uint64_t
xen::entity_mgr::num_entities ()
{
  return m_counter;
}
