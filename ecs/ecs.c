#include "ecs.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

eid_t
create_entity (struct ecs *ecs)
{
  /* TODO: do we ever need to delete an entity? */
  assert (ecs->num_entities != UINT64_MAX);
  return ecs->num_entities++;
}

cid_t
create_component_array (struct ecs *ecs, size_t size, size_t nmemb)
{
  size_t newsize
      = sizeof (struct component_array) * (ecs->num_component_types + 1);
  ecs->arrays = realloc (ecs->arrays, newsize);
  assert (ecs->arrays != NULL);

  size_t bufsize = size * nmemb;
  size_t mapsize = sizeof (struct offset_map) * nmemb;
  void *mem = malloc (bufsize + mapsize);
  ecs->arrays[ecs->num_component_types].buf = mem;
  ecs->arrays[ecs->num_component_types].map = mem + bufsize;
  memset (ecs->arrays[ecs->num_component_types].buf, 0, size * nmemb);
  ecs->arrays[ecs->num_component_types].bufsize = size * nmemb;
  ecs->arrays[ecs->num_component_types].stride = size;
  ecs->arrays[ecs->num_component_types].num_components = 0;
  return ecs->num_component_types++;
}

int
assign_component (struct ecs *ecs, eid_t entity, cid_t component)
{
  struct component_array *array = &ecs->arrays[component];
  if (array->num_components * array->stride == array->bufsize)
    {
      return -1;
    }

  array->map[array->num_components].entity = entity;
  array->map[array->num_components].offset
      = array->num_components * array->stride;
  ++array->num_components;
  return 0;
}
