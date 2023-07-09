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
map_component (struct ecs *ecs, eid_t entity, cid_t component)
{
  struct component_array *array = &ecs->arrays[component];
  if (array->num_components * array->stride == array->bufsize)
    {
      return -1;
    }

  array->map[array->num_components].entity = entity;
  array->map[array->num_components].offset
      = array->num_components * array->stride;
  array->num_components++;
  return 0;
}

int get_last (struct offset_map *map, struct component_array *arr)
{
  size_t last_offset = (arr->num_components - 1) * arr->stride;
  for (size_t i = 0; i < arr->num_components; i++)
    {
      if (arr->map[i].offset == last_offset)
        {
          *map = arr->map[i];
          return 0;
        }
    }
  return -1;
}

void
unmap_component (struct ecs *ecs, eid_t entity, cid_t component)
{
  /* TODO: Is this the best way of doing this? */
  struct component_array *array = &ecs->arrays[component];
  for (size_t i = 0; i < array->num_components; i++)
    {
      if (array->map[i].entity == entity)
        {
          struct offset_map last_mapping;
          assert (get_last (&last_mapping, array) == 0);
          size_t buffer_offset_to_delete = array->map[i].offset;
          memcpy (array->buf + buffer_offset_to_delete,
                  array->buf + last_mapping.offset, array->stride);
          last_mapping.offset = buffer_offset_to_delete;
          array->map[i] = last_mapping;
          --array->num_components;
          unmap_component (ecs, entity, component);
        }
    }
}
