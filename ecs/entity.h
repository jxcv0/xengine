#ifndef ENTITY_H_
#define ENTITY_H_

#include <array>
#include <cassert>
#include <cstdint>
#include <queue>
#include <stdexcept>

using EntityHandle = std::uint32_t;
using Signature = std::uint16_t;

/**
 * @brief Entity system
 */
template <std::size_t N = 128>
class EntitySystem {
 public:
  /**
   * @brief Construct an EntitySystem object. Populate the free list with
   *        available entity ids that are indexes in the signatures array.
   */
  EntitySystem() {
    for (auto i = 0; i < N; i++) {
      m_free_list.push(i);
    }
  }

  ~EntitySystem() = default;

  /**
   * @brief Create an entity.
   *
   * @return A new entity handle.
   */
  EntityHandle create_entity() {
    if (m_free_list.empty()) {
      throw std::runtime_error("maximum number of entities reached");
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
  Signature get_signature(EntityHandle e) {
    assert(e < N);
    return m_signatures[e];
  }

  /**
   * @brief Set the signature of an entity.
   *
   * @param e The handle of the entity.
   * @param s The new signature to assign to the entity.
   */
  void set_signature(EntityHandle e, Signature s) { m_signatures[e] = s; }

  /**
   * @brief Bitwise AND an entity's signature and a new signature.
   *
   * @param e The handle of the entity.
   * @param s The signature to combine.
   */
  void amend_signature(EntityHandle e, Signature s) { m_signatures[e] &= s; }

  /**
   * @brief Erase and entity. Put it's handle onto the free list and set it's
   *        index in the signature array to 0.
   *
   * @param e The entity to erase.
   */
  void erase_entity(EntityHandle e) {
    m_signatures[e] = 0;
    m_free_list.push(e);
  }

 private:
  std::queue<EntityHandle> m_free_list;
  std::array<Signature, N> m_signatures{0};
};

#endif  // ENTITY_H_
