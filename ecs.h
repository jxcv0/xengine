#ifndef ECS_H_
#define ECS_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* entity id */
typedef uint64_t eid_t;

/* component id */
typedef uint64_t cid_t;

struct component_array
{
  void *buf;
  size_t bufsize;
  size_t num_components;
  size_t stride;
};

struct ecs
{
  struct component_array *component_arrays;
  size_t num_component_types;
};

/* Create a new entity */
eid_t create_entity (struct ecs *ecs);

/* Create a new component array */
cid_t create_component_array (struct ecs *ecs, size_t size, size_t nmemb);

#ifdef __cplusplus
} /* end of extern "C"
#endif

#endif  // ECS_H_
