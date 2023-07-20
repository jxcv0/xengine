#include "archetype.hpp"
#include <cstring>

xen::archetype::archetype(const std::vector<xen::component_info>& components)
{
  for (const auto& info : components)
    {
      m_components.push_back({ info.index, info.size, nullptr });
    }
}

bool
xen::archetype::has_component(const std::type_index& index) const
{
  const auto& it
      = std::find_if(m_components.cbegin(), m_components.cend(),
                     [=](const auto& row) { return row.index == index; });
  return it != m_components.cend();
}

std::size_t
xen::archetype::components_size() const
{
  std::size_t size = 0;
  for (const auto& row : m_components)
    {
      size += row.size;
    }
  return size;
}

std::size_t
xen::archetype::num_entities() const
{
  return m_entities.size();
}

void
xen::archetype::add_entity(eid_t entity)
{
  /*
   * TODO: This can be made into a single malloc with some pointer arithmetic
   * Each row needs to be shifted down (row.size * <index in m_components>)
   * bytes in this archetypes void *m_mem.
   *
   * TODO: Alignment and true sharing.
   */
  for (auto& row : m_components)
    {
      void* mem = std::realloc(row.data, (m_entities.size() + 1) * row.size);
      if (mem == nullptr)
        {
          throw std::bad_alloc();
        }
      row.data = static_cast<std::byte*>(mem);
    }
  m_entities.push_back(entity);
}

void
xen::archetype::remove_entity(eid_t entity)
{
  std::size_t index = get_index(entity);
  for (auto& row : m_components)
    {
      std::byte* data = static_cast<std::byte*>(row.data);
      std::size_t offset_of_deleted = row.size * index;
      std::size_t offset_of_next = row.size * (index + 1);
      std::size_t data_size = row.size * m_entities.size();
      std::size_t size_to_move = data_size - offset_of_next;
      std::memmove(data + offset_of_next, data + offset_of_deleted, size_to_move);
    }
  m_entities.erase(std::find(m_entities.cbegin(), m_entities.cend(), entity));
}

std::size_t
xen::archetype::get_index(eid_t entity) const
{
  return std::distance(
      m_entities.cbegin(),
      std::find(m_entities.cbegin(), m_entities.cend(), entity));
}

void*
xen::archetype::get_component(eid_t entity, const std::type_index& index)
{
  auto it = std::find_if(m_components.begin(), m_components.end(),
                         [&](const auto& row) { return row.index == index; });
  return it->data + (it->size * get_index(entity));
}

std::vector<xen::component_info>
xen::archetype::get_component_info() const
{
  std::vector<component_info> cinfo{};
  for (const auto& row : m_components)
    {
      cinfo.push_back({ row.index, row.size });
    }
  return cinfo;
}