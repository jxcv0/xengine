#include "entity_mgr.hpp"

#include <algorithm>

xen::eid_t xen::entity_mgr::create_entity() {
  eid_t new_id;
  if (!m_freelist.empty()) {
    new_id = m_freelist.back();
    m_freelist.pop_back();
  } else {
    new_id = m_counter++;
  }
  return new_id;
}

void xen::entity_mgr::delete_entity(eid_t entity) {
  m_freelist.push_back(entity);
}

std::size_t xen::entity_mgr::num_entities() const {
  return m_counter - m_freelist.size();
}

bool xen::entity_mgr::is_valid_id(eid_t entity) const {
  auto it = std::find(m_freelist.cbegin(), m_freelist.cend(), entity);
  return entity < m_counter && it == m_freelist.cend();
}