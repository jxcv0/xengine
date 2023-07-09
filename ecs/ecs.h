#ifndef ECS_H_
#define ECS_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* entity id */
typedef uint64_t eid_t;

/* component id */
typedef uint64_t cid_t;

struct offset_map
{
  eid_t entity;
  size_t offset;
};

struct component_array
{
  void *buf;
  struct offset_map *map;
  size_t num_components;
  size_t bufsize;
  size_t stride;
};

struct ecs
{
  eid_t num_entities;
  struct component_array *arrays;
  size_t num_component_types;
};

/* Create a new entity */
eid_t create_entity (struct ecs *ecs);

/* Create a new component array */
cid_t create_component_array (struct ecs *ecs, size_t size, size_t nmemb);

/* Map an entity id to the next available component in a component array */
int assign_component (struct ecs *ecs, eid_t entity, cid_t component);

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif // ECS_H_
