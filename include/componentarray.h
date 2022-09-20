#ifndef COMPONENTARRAY_H_
#define COMPONENTARRAY_H_

#include <cstdint>
#include "entitysubsystem.h"

/**
 * @brief Components are identified by a unique id
 */
using cid_t = std::uint32_t;

/**
 * @brief Manages a contiguous array of components and keeps track of the association between
 *        entities and components.
 */
template<typename Component>
class ComponentArray {
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
    cid_t update(eid_t entity, Component component);

    /**
     * @brief Remove the association between a component and an entity.
     *
     * @param entity The entity to reset.
     */
    void reset(eid_t entity);

    /**
     * @brief Get a reference to an entities component.
     *
     * @param entity The entity.
     * @return The component associated with the entity.
     */
    Component& get(eid_t entity);

private:
    std::array<Component, MAX_COMPONENTS> m_components;
    std::map<eid_t, cid_t> m_entity_to_component;
    std::map<cid_t, eid_t> m_component_to_entity;
    std::uint32_t m_num_components = 0;
};

#endif // COMPONENTARRAY_H_
