#ifndef ARCHETYPE_H_
#define ARCHETYPE_H_

#include <array>
#include <cstddef>
#include <cstdlib>
#include <stdexcept>
#include <typeindex>
#include <vector>

#include "types.hpp"

namespace xen {

/**
 * @brief Stores components as an array of bytes.
 *
 */
class row {
 public:
  row(const std::type_index& index, std::size_t size)
      : m_index{index}, m_stride{size}, m_data{} {}

  std::type_index index() const { return m_index; }

  void* operator[](std::size_t i) { return m_data.data() + (i * m_stride); }

  void add() {
    /* TODO: Is there another way to do this? */
    for (std::size_t i = 0; i < m_stride; i++) {
      m_data.push_back(std::byte{});
    }
  }

  void remove(std::size_t index) {
    auto begin = m_data.cbegin() + (index * m_stride);
    auto end = begin + m_stride;
    m_data.erase(begin, end);
  }

  std::size_t stride() const { return m_stride; }

  std::size_t size() const { return m_data.size() / m_stride; }

  template <typename T>
  T& get_as(std::size_t index) {
    if (std::type_index(typeid(T)) != m_index) {
      throw std::runtime_error("Invalid type requested from row");
    }
    return *reinterpret_cast<T*>((*this)[index]);
  }

 private:
  const std::type_index m_index;
  const std::size_t m_stride;
  std::vector<std::byte> m_data;
};

struct component_info {
  std::type_index index;
  std::size_t size;

  /**
   * @brief Compare equality of two component_info structures.
   *
   * @param i1
   * @param i2
   * @return true if both structures contain info about the same type.
   * @return false if the structure contain info about different types.
   */
  friend bool operator==(const component_info& i1, const component_info& i2) {
    return i1.index == i2.index && i1.size == i2.size;
  }
};

template <typename... ComponentTs>
std::vector<component_info> create_component_info() {
  std::vector<component_info> res{};
  ((res.push_back({std::type_index(typeid(ComponentTs)), sizeof(ComponentTs)})),
   ...);
  return res;
}

class archetype {
 public:
  /**
   * @brief Construct a new archetype object.
   *
   * @param components The component_info of the types this component has.
   */
  archetype(const std::vector<component_info>& components);

  /**
   * @brief Check if the archetype contains a component type.
   *
   * @param index The type index of the component type.
   * @return true
   * @return false
   */
  bool has_component(const std::type_index& index) const;

  /**
   * @brief Check if the archetype has a component.
   *
   * @tparam Component The type of the component.
   * @return true
   * @return false
   */
  template <typename Component>
  bool has_component() const {
    return has_component(std::type_index(typeid(Component)));
  }

  /**
   * @brief Check if the archetype has multiple components.
   *
   * @tparam ComponentTs The types of the components.
   * @return true
   * @return false
   */
  template <typename... ComponentTs>
  bool has_components() const {
    std::size_t hascount = 0;
    (
        [&] {
          if (has_component<ComponentTs>()) {
            ++hascount;
          }
        }(),
        ...);
    return hascount == sizeof...(ComponentTs) &&
           m_components.size() == sizeof...(ComponentTs);
  }

  /**
   * @brief Get the combined size of the components in the archtype.
   *
   * @return std::size_t
   */
  std::size_t components_size() const;

  std::size_t size_of(const std::type_index& index) const;

  /**
   * @brief Get the number of entities of the archetype.
   *
   * @return std::size_t
   */
  std::size_t num_entities() const;

  /**
   * @brief Get the number of component types of the archetype.
   *
   * @return std::size_t
   */
  std::size_t num_components() const;

  /**
   * @brief Add an entity to the archetype.
   *
   * @param entity The entity to add.
   */
  void add_entity(eid_t entity);

  /**
   * @brief Remove an entity from the archetype.
   *
   * @param entity The entity to remove.
   */
  void remove_entity(eid_t entity);

  /**
   * @brief Check if an entity is of the archetype.
   *
   * @param entity The entity to search for.
   * @return true
   * @return false
   */
  bool has_entity(eid_t entity) const;

  /**
   * @brief Get the index object.
   *
   * @param entity
   * @return std::size_t
   */
  std::size_t get_index(eid_t entity) const;

  /**
   * @brief Get a component belonging to an entity with a type index.
   *
   * @param entity The entity the component belongs to.
   * @param index The type index of the component type.
   * @return void*
   */
  void* get_component(eid_t entity, const std::type_index& index);

  /**
   * @brief Get a component belonging to an entity.
   *
   * @tparam Component The type of the component
   * @param entity The entity of the component
   * @return Component&
   */
  template <typename Component>
  Component& get_component(eid_t entity) {
    return *static_cast<Component*>(
        get_component(entity, std::type_index(typeid(Component))));
  }

  /**
   * @brief Get the component info of the types stored in this archetype.
   *
   * @return std::vector<component_info>
   */
  std::vector<component_info> get_component_info() const;

 private:
  std::vector<row> m_components;
  std::vector<eid_t> m_entities;
};

} /* end of namespace xen */

#endif /* ARCHETYPE_H_ */
