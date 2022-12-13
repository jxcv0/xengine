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
   * @brief Get an entities signature by it's handle.
   *
   * @param e The entity handle.
   * @return The signature of the entity.
   */
  int get_signature(int e) const noexcept {
    assert(static_cast<unsigned int>(e) < N);
    return m_signatures[e];
  }

  /**
   * @brief Set the signature of an entity.
   *
   * @param e The handle of the entity.
   * @param s The new signature to assign to the entity.
   */
  void set_signature(int e, int s) noexcept { m_signatures[e] = s; }

  /**
   * @brief Bitwise AND an entity's signature and a new signature.
   *
   * @param e The handle of the entity.
   * @param s The signature to combine.
   */
  void amend_signature(int e, int s) noexcept { m_signatures[e] &= s; }

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
