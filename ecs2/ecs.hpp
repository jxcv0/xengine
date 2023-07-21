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
  eid_t create_entity();

  eid_t num_entities() const { return m_entities.num_entities(); }

  eid_t num_archetypes() const { return m_archetypes.size(); }

  template <typename... ComponentTs>
  bool has_components(eid_t entity) const {
    validate_id(entity);
    auto arch_it = find_archetype(entity);
    if (arch_it == m_archetypes.end()) {
      return false;
    }
    if (arch_it->template has_components<ComponentTs...>()) {
      return false;
    }
    return true;
  }

  template <typename Component>
  void add_component(eid_t entity) {
    if (has_components<Component>(entity)) {
      return;
    }
  }

 private:
  void validate_id(eid_t entity) const {
    if (!m_entities.is_valid_id(entity)) {
      throw std::invalid_argument("Invalid entity ID");
    }
  }

  auto find_archetype(eid_t entity) const {
    return std::find_if(
        m_archetypes.begin(), m_archetypes.end(),
        [=](const auto& arch) { return arch.has_entity(entity); });
  }

 private:
  entity_mgr m_entities;
  std::vector<archetype> m_archetypes;
};

} /* end of namespace xen */

#endif /* ECS_HPP_ */
