#ifndef ENTITYSUBSYSTEM_H_
#define ENTITYSUBSYSTEM_H_

#include <array>
#include <bitset>
#include <cstdint>
#include <glm/fwd.hpp>
#include <queue>

const std::uint32_t MAX_ENTITIES = 50000;
const std::uint8_t MAX_COMPONENTS = 32;

/**
 * @brief eid_t's are used to acces the signature array to find the id of it's
 *        associated components
 *
 */
using eid_t = std::uint32_t;

/**
 * @brief Each bit represents an entity type. If the entity is associeted with
 *        a type represented by bit 3 then that signatures 3rd bit will be 1.
 *        If an entity does not use a component then that position will be 0.
 */
using signature_t = std::bitset<MAX_COMPONENTS>;

/**
 * @brief Manages the entities and thier signatures.
 */
class EntitySubsystem {
 public:
  /**
   * @brief Populate the free list with all possible eid's.
   */
  EntitySubsystem();

  /**
   * @brief Create a new entity and assign it a unique id.
   *
   * @return The id of the new entity.
   */
  eid_t create_entity();

  /**
   * @brief Create an entity with a signature.
   *
   * @param sig The signature of the entity.
   * @return The id of the new entity.
   */
  eid_t create_entity(signature_t sig);

  /**
   * @brief Remove and entity from circulaton and make the id available to new
   *        entities.
   *
   * @param id The id of the entity to destroy.
   */
  void destroy_entity(eid_t entity);

  /**
   * @brief Set the signature of an entity.
   *
   * @param enitity The entity.
   * @param sig The new signature.
   */
  void set_signature(eid_t enitity, signature_t sig);

  /**
   * @brief Get the signature of an entity.
   *
   * @param entity The entity.
   * @return The signature of the entity.
   */
  signature_t get_signature(eid_t entity);

 private:
  std::queue<eid_t> m_available_eids;
  std::array<signature_t, MAX_ENTITIES> m_signatures;
  std::uint32_t m_num_entities = 0;
};

#endif  // ENTITYSUBSYSTEM_H_
