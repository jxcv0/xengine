#ifndef COMPONENTARRAY_H_
#define COMPONENTARRAY_H_

#include <cstdint>

#include "constants.h"

namespace xen {

template <typename ComponentType>
class ComponentArray {
  static constexpr int NOT_IN_USE = -1;

 public:
  constexpr inline ComponentArray() noexcept {
    for (int i = 0; i < MAX_ENTITIES; i++) {
      m_map[i] = -1;
    }
  }

  constexpr int assign(const entity_id id) noexcept {
    if (m_map[id] == NOT_IN_USE && id < MAX_ENTITIES &&
        m_num_components != MAX_ENTITIES) {
      m_map[id] = m_num_components;
      return 0;
    }
    return -1;
  }

  constexpr int erase(const entity_id id) noexcept {
    if (m_map[id] != NOT_IN_USE && id < MAX_ENTITIES && m_num_components != 0) {
      int index = m_map[id];
      if (index != m_num_components) {
        // if the deleted is not last we swap
        m_components[index] = m_components[m_num_components];
        m_map[index_to_id(index)] = index;
        m_map[id] = NOT_IN_USE;
        m_num_components--;
      } else {
        // if it is last then we can just remove it
        m_map[id] = NOT_IN_USE;
        m_num_components--;
      }
      return 0;
    }
    return -1;
  }

  constexpr ComponentType *get(const entity_id id) const noexcept {
    if (m_map[id] != NOT_IN_USE) {
      return m_components[m_map[id]];
    }
    return nullptr;
  }

 private:
  constexpr int index_to_id(const int index) const noexcept {
    for (int i = 0; i < m_num_components; i++) {
      if (m_map[i] == index) return i;
    }
    return -1;
  }

  int m_map[MAX_ENTITIES];
  int m_num_components = 0;
  ComponentType m_components[MAX_ENTITIES];
};

}  // namespace xen

#endif  // COMPONENTARRAY_H_
