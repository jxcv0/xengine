#ifndef MEM_H_
#define MEM_H_

#include <stddef.h>

#include "components.h"

#define NUM_COMPONENT_TYPES 63
#define MAX_NUM_ENTITIES 2048
#define ENTITY_UNUSED (1LU << NUM_COMPONENT_TYPES)

#define MAX_NUM_GEOMETRIES 32
#define MAX_NUM_MATERIALS 32
#define MAX_NUM_POSITIONS 32
#define MAX_NUM_MODEL_MATRICES 32
#define MAX_NUM_LOAD_REQUESTS 32

/**
 * @brief Initialize the memory subsystem.
 */
void init_mem_subsys(void);

/**
 * @brief Create a mask from an array of components.
 *
 * @param n The number of members in the components array
 * @param components An array of component types.
 * @return A The identity (bitmask) of entities that have all the components in
 * the components array.
 */
uint64_t create_mask(size_t n, uint64_t *components);

/**
 * @brief Create an entity.
 * @param e A pointer to the entity handle to initialize.
 * @return 0 on success, -1 on failure.
 */
int create_entity(uint32_t *e);

/**
 * @brief Delete an entity.
 * @details If the entity does not exist then this function does nothing.
 */
void delete_entity(uint32_t e);

/**
 * @brief Get the identity (bitmask) of an entity.
 * @param e The entity.
 * @return A mask of all the components this entity has an instance of.
 */
uint64_t get_identity(uint32_t e);

/**
 * @brief Assign a component to an entity.
 *
 * @param e The entity.
 * @param type The type of the component to assign.
 */
int add_component(uint32_t e, uint64_t type);

/**
 * @brief Remove a component from an entity.
 *
 * @param e The entity.
 * @param type The type of the component to remove.
 */
void remove_component(uint32_t e, uint64_t type);

/**
 * @brief Set the value of a component.
 *
 * @param e The entity.
 * @param type The type of the component to set.
 * @param cmpnt The new component value.
 * @return 0 on success, -1 on error.
 */
int set_component(uint32_t e, uint64_t type, union component cmpnt);

union component *get_component(uint32_t e, uint64_t type);

size_t get_component_count(uint64_t type);

/**
 * @brief Count the number of entities who's identity matches mask.
 * @param Mask The component bitmask.
 * @return The number of entities.
 */
size_t get_num_entities(uint64_t mask);

/**
 * @brief Get all entities with the same identity (mask). It is assumed that
 * \link get_num_entities has been called to determine the appropriate size of
 * get_entities.
 *
 * @param mask The identity bitmask of the entities.
 * @param arr The array to load the entities into.
 */
void get_entities(uint64_t mask, uint32_t *arr);

/**
 * @brief Generate a set of components of the same type belonging to entities.
 * The component at index n of array belongs to the entity at index n of
 * entities.
 *
 * @param nent The number of entities in the entities array.
 * @param type The type of the components.
 * @param set The destination array of components.
 */
void query(size_t nent, uint32_t *entities, uint64_t type,
           union component *set);

void update(size_t nent, uint32_t *entities, uint64_t type,
            union component *set);

#endif  // MEM_H_
