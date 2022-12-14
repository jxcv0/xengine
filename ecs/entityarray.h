#ifndef ENTITY_H_
#define ENTITY_H_

#include <array>
#include <cassert>
#include <cstdint>
#include <queue>
#include <stdexcept>

#include "components.h"

using EntityId = int;

/**
 * @brief Entity system
 */
template <int N = 256>
class EntityArray {
 public:
  /**
   * @brief Construct an EntityArray object. Populate the free list with
   *        available entity ids that are indexes in the signatures array.
   */
  constexpr inline EntityArray() noexcept {
    static_assert(N > 0);
    for (unsigned int i = 0; i < N; i++) {
      m_free_list.push(i);
    }
  }

  ~EntityArray() = default;

  auto count() const { return (N - m_free_list.size()); }

  /**
   * @brief Create an entity.
   *
   * @return A new entity handle.
   */
  int create_entity() noexcept {
    if (m_free_list.empty()) {
      return -1;
    }
    auto e = m_free_list.front();
    m_free_list.pop();
    return e;
  }

  bool has_component(EntityId e, ComponentId c) const noexcept {
    auto archetype = m_signatures[e];
    if ((archetype & c) != 0) {
      return true;
    }
    return false;
  }

  /**
   * @brief Bitwise AND an entity's signature and a new signature.
   *
   * @param e The handle of the entity.
   * @param s The signature to combine.
   */
  void add_component(int e, int s) noexcept { m_signatures[e] |= s; }

  /**
   * @brief Erase and entity. Put it's handle onto the free list and set it's
   *        index in the signature array to 0.
   *
   * @param e The entity to erase.
   */
  void erase_entity(int e) noexcept {
    m_signatures[e] = 0;  // reset signature
    m_free_list.push(e);
  }

 private:
  std::queue<int> m_free_list;
  int m_signatures[N] = {0};
};

#endif  // ENTITY_H_
