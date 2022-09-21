#ifndef COMPONENTARRAY_H_
#define COMPONENTARRAY_H_

#include <cassert>
#include <cstdint>
#include <map>
#include <stdexcept>

#include "entitysubsystem.h"

/**
 * @brief Components are identified by a unique id
 */
using cid_t = std::uint32_t;

const auto ERR_NOT_FOUND = MAX_ENTITIES + 1;

/**
 * @brief Interface to notify Component arrays of different types of the
 * destruction of an entity.
 */
class i_ComponentArray {
 public:
  virtual ~i_ComponentArray() = default;

  /**
   * @brief Notify the array that an entity has been destroyed.
   *
   * @param entity The id of the entity.
   */
  virtual void notify_destroyed(eid_t entity) = 0;
};

/**
 * @brief Manages a contiguous array of components and keeps track of the
 * association between entities and components.
 */
template <typename Component>
class ComponentArray : public i_ComponentArray {
 public:
  ComponentArray() = default;
  ~ComponentArray() = default;

  /**
   * @brief Update an enities component.
   *
   * @param entity The entity to add the component to.
   * @param component The component.
   * @return The id of the component.
   */
  cid_t add(eid_t entity, Component component) {
    assert(!m_entity_to_idx.contains(entity));
    auto idx = m_num_components;
    m_entity_to_idx[entity] = idx;
    m_idx_to_entity[idx] = entity;
    m_components[idx] = component;
    ++m_num_components;
  }

  /**
   * @brief Remove the association between a component and an entity.
   *        Reseting components maintains the contiguity of the data in the
   *        array.
   *
   * @param entity The entity to reset.
   */
  void reset(eid_t entity) {
    assert(m_entity_to_idx.contains(entity));

    // copy to last element into removed idx to defragment
    auto idx_removed = m_entity_to_idx[entity];
    auto last = m_num_components - 1;
    m_components[idx_removed] = m_components[last];

    // update mapping
    auto last_entity = m_idx_to_entity[last];
    m_entity_to_idx[last_entity] = idx_removed;
    m_idx_to_entity[idx_removed] = last_entity;

    // remove dangling entries
    m_entity_to_idx.erase(entity);
    m_idx_to_entity.erase(idx_removed);
    --m_num_components;
  }

  /**
   * @brief Get a reference to an entities component.
   *
   * @param entity The entity.
   * @return The component associated with the entity.
   */
  Component &get(eid_t entity) {
    assert(m_entity_to_idx.contains(entity));
    return &m_entity_to_idx[entity];
  }

  /**
   * @brief Implimentation of virtual i_ComponentArray.
   */
  void notify_destroyed(eid_t entity) override {
    if (m_entity_to_idx.contains(entity)) {
      reset(entity);
    }
  };

 private:
  /**
   * @brief An entity and its associated component.
   */
  struct ECPair {
    eid_t m_entity;
    cid_t m_component;
  };

  /**
   * @Brief Get the index of an entities associated component (it's id).
   *
   * @param entity The entity id.
   * @return The id of the component if successful otherwise and error
   *         constant.
   */
  cid_t find_cid(eid_t entity) const {
    for (const auto &pair : m_map) {
      if (pair.m_entity == entity) {
        return pair.m_component;
      }
      return ERR_NOT_FOUND;
    }
  }

  /**
   * @Brief Get a component's associated entity
   *
   * @param component The id of the component.
   * @return The id of the entity if successful otherwise and error constant.
   */
  eid_t find_eid(cid_t component) const {
    for (const auto &pair : m_map) {
      if (pair.m_component == component) {
        return pair.m_entity;
      }
      return ERR_NOT_FOUND;
    }
  }

  /**
   * @brief Store an entity component relationship. If the entity id is
   *        already stored in the map then its component is overwritten.
   *
   * @param entity The entity id.
   * @param component The component id.
   */
  void assign(eid_t entity, cid_t component) {
    auto it = std::find_if(m_map.begin(), m_map.end(), [=](const auto &pair) {
      return pair.m_entity == entity;
    });
    auto entity_id = find_eid(entity);
    if (it == m_map.end()) {
      m_map.push_back({entity, component});
    } else {
      it.m_component = component;
    }
  }

  /**
   * @brief Remove a pair from the map
   *
   * @param entity The entity of the pair to remove.
   */
  void remove(eid_t entity) {
    auto it = std::find_if(m_map.begin(), m_map.end(), [=](const auto &pair) {
      return pair.m_entity == entity;
    });
    if (it != m_map.end()) {
      m_components.erase(it);
    }
  }

  // An array could also be used here but this may use less memory as not all
  // entities require all component types.
  std::vector<ECPair> m_map;

  std::array<Component, MAX_COMPONENTS> m_components;
  std::map<eid_t, cid_t> m_entity_to_idx;
  std::map<cid_t, eid_t> m_idx_to_entity;
  std::uint32_t m_num_components = 0;
};

#endif  // COMPONENTARRAY_H_
