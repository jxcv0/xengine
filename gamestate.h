#ifndef MEM_H_
#define MEM_H_

#include <stddef.h>

#include "attrib.h"

#define NUM_COMPONENT_TYPES 63
#define MAX_NUM_ENTITIES 2048
#define MAX_NUM_ATTRIBS 63
#define ENTITY_UNUSED (1LU << NUM_COMPONENT_TYPES)
#define ENTITY_IN_USE (1LU << NUM_COMPONENT_TYPES)

#define MAX_NUM_GEOMETRIES 32
#define MAX_NUM_MATERIALS 32
#define MAX_NUM_POSITIONS 32
#define MAX_NUM_MODEL_MATRICES 32
#define MAX_NUM_LOAD_REQUESTS 32

/**
 * @brief Initialize the game state subsystem.
 */
void init_gamestate(void);

/**
 * @brief Create a mask from an array of attribute type ids.
 *
 * @param n The number of members in the components array
 * @param attrib_types An array of attribute types types.
 * @return A The identity (bitmask) of entities that have all the components in
 * the components array.
 */
uint64_t create_mask(size_t n, uint64_t *attrib_types);

/**
 * @brief Create a game object.
 *
 * @param oid A pointer to where to store the object id.
 * @return 0 on success -1, on error.
 */
int create_obj(uint32_t *oid);

/**
 * @brief Delete a game object.
 *
 * @param obj The object to delete.
 */
void delete_obj(uint32_t obj);

/**
 * @brief Get the attribute bitmask of an object.
 *
 * @param obj The game object.
 * @return The attribute mask of obj.
 */
uint64_t get_attribs(uint32_t obj);

/**
 * @brief Assign a attribute to an object.
 *
 * @param obj The type of the attribute to assign.
 * @param attrib_type The type of attribute to assign.
 * @return 0 on success, -1 on error.
 */
int add_attrib(uint32_t obj, uint64_t attrib_type);

/**
 * @brief Remove a attribute from an object.
 *
 * @param obj The object.
 * @param attrib_type The type of the attribute to remove.
 */
void remove_attrib(uint32_t e, uint64_t attrib_type);

/**
 * @brief Set the value of a attribute.
 *
 * @param obj The object.
 * @param type The type of the attribute to set.
 * @param attrib The new attribute value.
 * @return 0 on success, -1 on error.
 */
int set_attrib(uint32_t obj, uint64_t attrib_type, attrib_t attrib);

/**
 * @brief Get an attribute of an object.
 *
 * @param obj The object.
 * @param attrib_type The type of the attribute.
 * @return The attribute.
 */
attrib_t get_attrib(uint32_t e, uint64_t attrib_type);

/**
 * @brief Get the number of attributes of a type.
 *
 * @param attrib_type The attribute type.
 * @return The number of attributes of attrib_type.
 */
size_t get_num_attribs(uint64_t attrib_type);

/**
 * @brief Count the number of objects who's attributes matches mask.
 *
 * @param attrib_mask The attribute bitmask.
 * @return The number of objects.
 */
size_t get_num_obj(uint64_t attrib_mask);

/**
 * @brief Get all objects with the same attribute mask.
 *
 * @param mask The identity bitmask of the entities.
 * @param arr The array to load the entities into.
 */
void get_objs(uint64_t mask, uint32_t *arr);

/**
 * @brief Generate a set of attributes of the same type belonging to a list of
 * objects. The attribute at index n of array belongs to the entity at index n
 * of entities.
 *
 * @param nent The number of objects in the objects array.
 * @param type The type of the attributes.
 * @param set The destination array of attributes.
 */
void query(size_t nobj, uint32_t *objs, uint64_t attrib_type,
           union attribute *attribs);

/**
 * @brief Save back a modified query.
 *
 * @param nobj The number of objects.
 * @param objs The objects
 * @param attrib_type The type of the attributes.
 * @param attribs The array of attributes.
 */
void update(size_t nobj, uint32_t *objs, uint64_t attrib_type,
            union attribute *attribs);

#endif  // MEM_H_
