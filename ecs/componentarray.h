#ifndef COMPONENTARRAY_H_
#define COMPONENTARRAY_H_

#include <cstdint>

#include "constants.h"

/**
 * @brief Contiguous storage for components accessed by entity id. For
 * simplicity, entities may have only entry in the array.
 */
template <typename ComponentType>
class ComponentArray {
 public:
  constexpr int assign(const entity_id id) noexcept {
    if (id_is_valid(id) && m_num_components != MAX_ENTITIES &&
        id_to_index(id) == -1) {
      m_entity_ids[m_num_components++] = id;
      return 0;
    }
    return -1;
  }

  constexpr void erase(const entity_id id) noexcept {
    int index_to_delete = id_to_index(id);
    if (id_is_valid(id) && m_num_components != 0 && index_to_delete != -1) {
      int last_id = m_entity_ids[m_num_components - 1];

      if (id != last_id) {
        // if the component is not last in the array we swap
        m_entity_ids[index_to_delete] = m_entity_ids[m_num_components - 1];
        m_components[index_to_delete] = m_components[m_num_components - 1];
      }
      m_num_components--;
    }
  }

  constexpr ComponentType *get(const entity_id id) noexcept {
    int index = id_to_index(id);
    if (index != -1) {
      return &m_components[index];
    }
    return nullptr;
  }

  constexpr int set(const entity_id id, ComponentType c) noexcept {
    if (id_is_valid(id) && m_num_components != MAX_COMPONENTS &&
        id_to_index(id) != -1) {
      int index = id_to_index(id);
      m_components[index] = c;
      return 0;
    }
    return -1;
  }

 private:
  int id_to_index(const entity_id id) {
    for (int i = 0; i < m_num_components; i++) {
      if (m_entity_ids[i] == id) {
        return i;
      }
    }
    return -1;
  }

  constexpr inline bool id_is_valid(const entity_id id) {
    return (id < MAX_ENTITIES && id > 0);
  }

  int m_num_components = 0;
  int m_entity_ids[MAX_ENTITIES];
  ComponentType m_components[MAX_ENTITIES];
};

#endif  // COMPONENTARRAY_H_
