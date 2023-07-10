#ifndef ECS_H_
#define ECS_H_

#include <stddef.h>
#include <stdint.h>

#define MAX_NUM_COMPONENT_TYPES 256

#ifdef __cplusplus
extern "C" {
#endif

/* entity id */
typedef uint64_t eid_t;

/* component id */
typedef uint64_t cid_t;

struct component_bitset
{
  uint64_t sets[4];
};

struct offset_pair
{
  eid_t entity;
  size_t offset;
};

struct component_array
{
  void *buf;
  struct offset_pair *map;
  size_t num_components;
  size_t bufsize;
  size_t stride;
};

struct ecs
{
  eid_t num_entities;
  struct component_array *arrays;
  size_t num_component_types;
  struct component_bitset *bitsets;
};

/* Create a new entity */
eid_t create_entity (struct ecs *ecs);

/* Create a new component array */
cid_t create_component_array (struct ecs *ecs, size_t size, size_t nmemb);

/* Set set component bit flag */
void set_bitset(struct component_bitset *bitset, cid_t component);

/* Unset the component bit flag for an entity */
void unset_bitset(struct component_bitset *bitset, cid_t component);

/* Map an entity id to the next available component in a component array TODO:
 * is it better to be able to assign more that one? */
int map_component (struct ecs *ecs, eid_t entity, cid_t component);

/* Unmap all components from an entity */
void unmap_component (struct ecs *ecs, eid_t entity, cid_t component);

/* Get a component by it's entity */
void *get_component (struct ecs *ecs, eid_t entity, cid_t component);

/* Check if an entity has a component */
int has_component (struct ecs *ecs, eid_t entity, cid_t component);

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif // ECS_H_
