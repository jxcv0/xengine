#ifndef COMPONENT_H_
#define COMPONENT_H_

#ifndef MAX_COMPONENTS
#define MAX_COMPONENTS 16
#endif

#include "entity.h"

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
 *
 *        note: Lessons learned. Simpler is better. Iterators are B.S.
 */
template <typename ComponentType, std::size_t N = MAX_ENTITIES>
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
    m_map[m_num_components].m_handle = e;
    m_map[m_num_components].m_index = m_num_components;
    m_components[m_num_components] = c;
    m_num_components++;
  }

  /**
   * @brief Remove the components and mapping related to an entity.
   *
   * @param e The enity to erase.
   */
  void erase_entity(EntityHandle e) override {
    for (int i = 0; i < N; i++) {
      if (m_map[i].m_handle == e) {
        m_map[i].m_index = -1;  // AKA not in use
      }
    }
  }

  /**
   * @brief Get the component associated with an entity.
   *
   * @param e The handle of the entity.
   * @return A pointer to the component stored in the array. If the component
   * is not found then nullptr is returned.
   */
  ComponentType* get_component(EntityHandle e) {
    for (int i = 0; i < N; i++) {
      if (m_map[i].m_handle == e) {
        int index = m_map[i].m_index;
        if (index != -1) {
          return &m_components[index];
        }
      }
    }
    return nullptr;
  }

 private:
  struct {
    EntityHandle m_handle;
    int m_index = -1;
  } m_map[N];

  ComponentType m_components[N];  // this is wasteful
  std::uint32_t m_num_components = 0;
};

#endif  // COMPONENT_H_
