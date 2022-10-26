#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "entity.h"
#include <array>
#include <unordered_map>

/**
 * @brief Base class for ComponentArray declaring the erase function.
 */
class ComponentArrayBase {
 public:
  virtual ~ComponentArrayBase() = default;

  /**
   * @brief Remove an entity from the array.
   *
   * @param e The handle of the entity that to erase.
   */
  virtual void erase_entity(EntityHandle e) = 0;
};

/**
 * @brief A contiguous array of components.
 *        Components can be assigned to an entity and accessed using an entity
 *        handle.
 */
template <typename ComponentType>
class ComponentArray : public ComponentArrayBase {
 public:
  ComponentArray() = default;
  virtual ~ComponentArray() = default;

  /**
   * @brief Assign a component to an entity
   *
   * @param e The entity to assign.
   * @param c The component to assign to the entity. 
   */
  void assign(EntityHandle e, ComponentType c) {
    if (m_num_components == MAX_ENTITIES) {
      throw std::runtime_error("maximum number of entities reached");
    }
    auto i = m_num_components;
    m_components[i] = c;
    m_ec_map[e] = i;
    m_num_components++;
  }

  void erase_entity(EntityHandle e) override {

  }

  /**
   * @brief Get the component associated with an entity.
   *
   * @param e The handle of the entity.
   * @return A reference to the component stored in the array.
   */
  ComponentType &get_component(EntityHandle e) {
  }


 private:
  std::unordered_map<EntityHandle, std::uint32_t> m_ec_map;
  std::array<ComponentType, MAX_ENTITIES> m_components{0};
  std::uint32_t m_num_components{0};
};

#endif // COMPONENT_H_
