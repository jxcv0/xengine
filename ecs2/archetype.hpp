#ifndef ARCHETYPE_H_
#define ARCHETYPE_H_

#include <cstddef>
#include <typeindex>
#include <vector>

#include "types.hpp"

namespace xen
{

struct component_info
{
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
  friend bool
  operator==(const component_info& i1, const component_info& i2)
  {
    return i1.index == i2.index && i1.size == i2.size;
  }
};

template <typename... ComponentTs>
std::vector<component_info>
create_component_info()
{
  std::vector<component_info> res{};
  ((res.push_back(
       { std::type_index(typeid(ComponentTs)), sizeof(ComponentTs) })),
   ...);
  return res;
}

class archetype
{
public:
  /**
   * @brief A row of one type of component with each entry belonging to a
   * unique entity.
   */
  struct row
  {
    std::type_index index;
    std::size_t size; /* Stride of the component */
    std::byte* data;
  };

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
  bool
  has_component() const
  {
    return has_component(std::type_index(typeid(Component)));
  }

  /**
   * @brief Get the combined size of the components in the archtype.
   *
   * @return std::size_t
   */
  std::size_t components_size() const;

  /**
   * @brief Get the number of entities of the archetype.
   *
   * @return std::size_t
   */
  std::size_t num_entities() const;

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
  Component&
  get_component(eid_t entity)
  {
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
