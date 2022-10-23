#ifndef COMPONENTARRAY_H_
#define COMPONENTARRAY_H_

#include <cassert>
#include <cstdint>
#include <map>
#include <stdexcept>
#include <type_traits>

#include "entitysubsystem.h"

/**
 * @brief Components are identified by a unique id
 */
using ComponentId = std::uint32_t;

/**
 * @brief Interface to notify Component arrays of different types of the
 *				destruction of an entity.
 */
class ComponentArrayBase {
 public:
  virtual ~ComponentArrayBase() = default;

  /**
   * @brief Notify the array that an entity has been destroyed.
   *
   * @param entity The id of the entity.
   */
  virtual void notify_destroyed(EntityId entity) = 0;
};

/**
 * @brief Manages a contiguous array of components and keeps track of the
 *				association between entities and components.
 */
template <typename Component>
class ComponentArray : public ComponentArrayBase {
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
  ComponentId add(EntityId entity, Component component) {
    assert(find_pair(entity) == m_map.end());
    auto new_cid = m_num_components;
    m_components[new_cid] = component;
    assign(entity, new_cid);
    ++m_num_components;
    return new_cid;
  }

  /**
   * @brief Remove the association between a component and an entity.
   *        Resetting components maintains the contiguity of the data in the
   *        array.
   *
   * @param entity The entity to reset.
   */
  void reset(EntityId entity) {
    remove(entity);
    --m_num_components;
  }

  /**
   * @brief Get a reference to an entities component.
   *
   * @param entity The entity.
   * @return The component associated with the entity.
   */
  Component &get(EntityId entity) {
    auto it = find_pair(entity);
    assert(it != m_map.end());
    auto pair = *it;
    auto component_id = pair.m_component;
    assert(component_id < m_num_components);
    return m_components[component_id];
  }

  /**
   * @brief Notify the array that an entity has been destroyed.
   *
   * @param entity The destroyed entity
   */
  void notify_destroyed(EntityId entity) override {
    if (find_pair(entity) != m_map.end()) {
      reset(entity);
    }
  };

 private:
  /**
   * @brief An entity and its associated component.
   */
  struct ECPair {
    EntityId m_entity;
    ComponentId m_component;
  };

  /**
   * @Brief Get an iterator to the map entry comtaining a component id. If the
   *        component is not found then the iterator will point to the end of
   *        the map.
   *
   * @param component The id of the component.
   * @return The iterator pointing to the entry or the end of the map.
   */
  auto find_pair(EntityId entity) const {
    return std::find_if(m_map.begin(), m_map.end(), [=](const auto &pair) {
      return pair.m_entity == entity;
    });
  }

  /**
   * @brief Store an entity component relationship. If the entity id is
   *        already stored in the map then its component is overwritten.
   *
   * @param entity The entity id.
   * @param component The component id.
   */
  void assign(EntityId entity, ComponentId component) {
    auto it = find_pair(entity);
    if (it == m_map.end()) {
      m_map.push_back({entity, component});
    } else {
      auto pair = *it;
      pair.m_component = component;
    }
  }

  /**
   * @brief Remove an entity from the map using an entity id.
   *
   * @param entity The entity of the pair to remove.
   */
  void remove(EntityId entity) {
    auto it = find_pair(entity);
    if (it != m_map.end()) {
      m_map.erase(it);
    }
  }

  // An array could also be used here but this may use less memory as not all
  // entities require all component types.
  std::vector<ECPair> m_map;
  std::array<Component, MAX_COMPONENTS> m_components;
  std::uint32_t m_num_components = 0;
};

#endif  // COMPONENTARRAY_H_
