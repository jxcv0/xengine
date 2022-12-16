#ifndef ENTITY_H_
#define ENTITY_H_

#include <array>
#include <cassert>
#include <cstdint>
#include <queue>
#include <stdexcept>

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

  /**
   * @brief Get the number of entities in use.
   *
   * @return The number of entities in use.
   */
  constexpr inline auto count() const { return (N - m_free_list.size()); }

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

  /**
   * @brief Check if an entity has a component.
   *
   * @brief e The entity id.
   * @brief c The component id.
   * @return true if the entity has the component, otherwise false.
   */
  constexpr bool has_component(int e, int c) const noexcept {
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
  constexpr inline void add_component(int e, int s) noexcept {
    m_signatures[e] |= s;
  }

  /**
   * @brief Erase and entity. Put it's handle onto the free list and set it's
   *        index in the signature array to 0.
   *
   * @param e The entity to erase.
   */
  constexpr void erase_entity(int e) noexcept {
    m_signatures[e] = 0;  // reset signature
    m_free_list.push(e);
  }

 private:
  std::queue<int> m_free_list;
  int m_signatures[N] = {0};
};

#endif  // ENTITY_H_
