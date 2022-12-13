#ifndef COMPONENT_H_
#define COMPONENT_H_

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
  virtual void erase_entity(int e) = 0;
};

/**
 * @brief A contiguous array of components.
 *        Components can be assigned to an entity and accessed using an entity
 *        handle.
 *
 *        note: Lessons learned. Simpler is better. Iterators are B.S.
 */
template <typename ComponentType, int N = 256>
class ComponentArray : public ComponentArrayBase {
 public:
  constexpr inline ComponentArray() noexcept { static_assert(N > 0); }
  virtual ~ComponentArray() = default;

  /**
   * @brief Assign a component to an entity
   *
   * @param e The entity to assign.
   * @param c The component to assign to the entity.
   */
  int assign(int e, ComponentType c) {
    if (m_components == N) {
      return -1;
    }
    m_map[m_num_components].m_handle = e;
    m_map[m_num_components].m_index = m_num_components;
    m_components[m_num_components] = c;
    m_num_components++;
    return 0;
  }

  /**
   * @brief Remove the components and mapping related to an entity.
   *
   * @param e The enity to erase.
   */
  void erase_entity(int e) override {
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
  ComponentType* get_component(int e) {
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

#ifndef COMPONENTARRAY_TEST
 private:
#endif
  struct {
    int m_handle;
    int m_index = -1;
  } m_map[N];

  ComponentType m_components[N];  // this is wasteful
  int m_num_components = 0;
};

#endif  // COMPONENT_H_
