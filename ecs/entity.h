#ifndef ENTITY_H_
#define ENTITY_H_

#include <cassert>
#include <cstdint>

/**
 * @brief An entity used to associate components.
 *				This class simply wraps an unsigned int.
 */
class Entity {
 public:
  using std::uint32_t = id_t;
  using std::uint16_t = signature_t;

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
  void update_signature(signature_t new_sig) { m_signature &= sig; }

 public:
  id_t m_id;
  signature_t m_signature
};

/**
 * @brief Entity system
 */
template <std::size_t N = 128>
class EntitySys {
 public:
  EntitySys() = default;
  ~EntitySys = default;

  // TODO

  /**
   * @brief Create an entity
   *
   * @return A new entity.
   */
  Entity create_entity() {
    assert(m_num_entities < N) auto e =
        m_entities[m_num_entities] m_num_entities++;
    return Entity(e);
  }

 private:
  std::array<Entity, N> m_entities;
  std::size_t m_num_entities;
};

#endif  // ENTITY_H_
