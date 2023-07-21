#include "archetype.hpp"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <stdexcept>

xen::archetype::archetype(const std::vector<xen::component_info>& components) {
  for (const auto& info : components) {
    m_components.push_back(row{info.index, info.size});
  }
}

bool xen::archetype::has_component(const std::type_index& index) const {
  const auto& it =
      std::find_if(m_components.cbegin(), m_components.cend(),
                   [=](const auto& row) { return row.index() == index; });
  return it != m_components.cend();
}

std::size_t xen::archetype::components_size() const {
  std::size_t size = 0;
  for (const auto& row : m_components) {
    size += row.stride();
  }
  return size;
}

std::size_t xen::archetype::size_of(const std::type_index& index) const {
  for (const auto& row : m_components) {
    if (row.index() == index) {
      return row.stride();
    }
  }
  return 0;
}

std::size_t xen::archetype::num_entities() const { return m_entities.size(); }

std::size_t xen::archetype::num_components() const {
  return m_components.size();
}

void xen::archetype::add_entity(eid_t entity) {
  /*
   * TODO: This can be made into a single malloc with some pointer arithmetic
   * Each row needs to be shifted down (row.size * <index in m_components>)
   * bytes in this archetypes void *m_mem.
   *
   * TODO: Alignment and true sharing.
   */
  for (auto& row : m_components) {
    row.add();
  }
  m_entities.push_back(entity);
}

void xen::archetype::remove_entity(eid_t entity) {
  std::size_t index = get_index(entity);
  for (auto& row : m_components) {
    row.remove(index);
  }
  m_entities.erase(std::find(m_entities.cbegin(), m_entities.cend(), entity));
}

bool xen::archetype::has_entity(eid_t entity) const {
  return m_entities.cend() !=
         std::find(m_entities.cbegin(), m_entities.cend(), entity);
}

std::size_t xen::archetype::get_index(eid_t entity) const {
  return std::distance(
      m_entities.cbegin(),
      std::find(m_entities.cbegin(), m_entities.cend(), entity));
}

void* xen::archetype::get_component(eid_t entity,
                                    const std::type_index& index) {
  auto it = std::find_if(m_components.begin(), m_components.end(),
                         [&](const auto& row) { return row.index() == index; });
  if (it == m_components.end()) {
    throw std::out_of_range("Component type not in archetype");
  }
  std::size_t entity_index = get_index(entity);
  row& row = *it;
  return row[entity_index];
}

std::vector<xen::component_info> xen::archetype::get_component_info() const {
  std::vector<component_info> cinfo{};
  for (const auto& row : m_components) {
    cinfo.push_back({row.index(), row.stride()});
  }
  return cinfo;
}