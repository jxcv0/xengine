#ifndef ENTITY_H_
#define ENTITY_H_

#include <array>
#include <cstdint>
#include <queue>
#include <stdexcept>

#include "entitysubsystem.h"

/**
 * @brief An entity used to associate components.
 *				This class simply wraps an unsigned int.
 */
class Entity {
 public:
  using id_t = std::uint32_t;
  using signature_t = std::uint16_t;

  /**
   * @brief Construct an entity from an id.
   *
   * @param id The id of the entity.
   */
  explicit Entity(id_t id) : m_id(id) {}

  /**
   * @brief Get the id of the entity.
   *
   * @return The id.
   */
  auto id() { return m_id; }

  /**
   * @brief Get the signature of the entity.
   *
   * @return The signature.
   */
  auto signature() { return m_signature; }

  /**
   * @brief Update the signature of the id.
   *
   * @param new_sig The new signature to and with the current.
   */
  void update_signature(signature_t new_sig) { m_signature &= new_sig; }

 public:
  id_t m_id;
  signature_t m_signature;
};

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
   * @return A new entity.
   */
  Entity create_entity() {
    if (m_num_entities == N) {
      throw std::runtime_error("maximum number of entities reached");
    }
    auto e = m_signatures[m_num_entities];
    m_num_entities++;
    return Entity(e);
  }

  // TODO

 private:
  std::queue<Entity::id_t> m_free_list;
  std::array<Entity::signature_t, N> m_signatures;
  std::size_t m_num_entities;
};

#endif  // ENTITY_H_
