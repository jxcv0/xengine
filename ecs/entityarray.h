#ifndef ENTITY_H_
#define ENTITY_H_

#include <array>
#include <cassert>
#include <cstdint>
#include <queue>
#include <stdexcept>

#include "constants.h"

/**
 * @brief Entity system
 */
class EntityArray {
 public:
  /**
   * @brief Construct an EntityArray object. Populate the free list with
   *        available entity ids that are indexes in the signatures array.
   */
  EntityArray() noexcept {
    for (unsigned int i = 1; i <= MAX_ENTITIES; i++) {  // 0 is not a valid id
      m_free_list.push(i);
    }
  }

  /**
   * @brief Get the number of entities in use.
   *
   * @return The number of entities in use.
   */
  auto count() const { return (MAX_ENTITIES - m_free_list.size()); }

  /**
   * @brief Create an entity.
   *
   * @return A new entity handle.
   */
  entity_id create() noexcept {
    if (m_free_list.empty()) {
      return -1;
    }
    auto e = m_free_list.front();
    m_free_list.pop();
    return e;
  }

  /**
   * @brief Check if an entity has a component.
   *
   * @brief e The entity id.
   * @brief c The component id.
   * @return true if the entity has the component, otherwise false.
   */
  constexpr bool has_component(entity_id e, int c) const noexcept {
    auto archetype = m_signatures[e];
    if ((archetype & c) != 0) {
      return true;
    }
    return false;
  }

  /**
   * @brief Bitwise OR an entity's signature and a new signature.
   *
   * @param e The handle of the entity.
   * @param s The signature to combine.
   */
  constexpr inline void add_components(entity_id e, int s) noexcept {
    m_signatures[e] |= s;
  }

  /**
   * @brief Erase and entity. Put it's handle onto the free list and set it's
   *        index in the signature array to 0.
   *
   * @param e The entity to erase.
   */
  void erase(entity_id e) noexcept {
    m_signatures[e] = 0;  // reset signature
    m_free_list.push(e);
  }

 private:
  std::queue<int> m_free_list;
  int m_signatures[MAX_ENTITIES] = {0};
};

#endif  // ENTITY_H_
