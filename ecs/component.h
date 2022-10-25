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
  void add(EntityHandle e, ComponentType c) {
    // TODO
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
  std::vector<ComponentType> m_components;
};

#endif // COMPONENT_H_
