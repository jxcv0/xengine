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
  void reset(eid_t entity) {
    remove(entity);
    --m_num_components;
  }

  /**
   * @brief Get a reference to an entities component.
   *
   * @param entity The entity.
   * @return The component associated with the entity.
   */
  Component &get(eid_t entity) {
    auto it = find_pair(entity);
    assert(it != m_map.end());
    return *it;
  }

  /**
   * @brief Implimentation of virtual i_ComponentArray.
   */
  void notify_destroyed(eid_t entity) override {
    if (find_pair(entity) != m_map.end()) {
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
   * @Brief Get an iterator to the map entry comtaining a component id. If the
   *        component is not found then the iterator will point to the end of
   *        the map.
   *
   * @param component The id of the component.
   * @return The iterator pointing to the entry or the end of the map.
   */
  auto find_pair(eid_t entity) const {
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
  void assign(eid_t entity, cid_t component) {
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
  void remove(eid_t entity) {
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

