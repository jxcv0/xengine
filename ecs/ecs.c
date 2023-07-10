#include "ecs.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

eid_t
create_entity (struct ecs *ecs)
{
  /* TODO: do we ever need to delete an entity? */
  assert (ecs->num_entities != UINT64_MAX);
  ecs->bitsets = realloc (ecs->bitsets, sizeof (struct component_bitset)
                                            * (ecs->num_entities + 1));
  memset (&ecs->bitsets[ecs->num_entities].sets, 0,
          sizeof (struct component_bitset));
  return ecs->num_entities++;
}

cid_t
create_component_array (struct ecs *ecs, size_t size, size_t nmemb)
{
  /* This can be increased later if required */
  assert (ecs->num_component_types != MAX_NUM_COMPONENT_TYPES);
  size_t newsize
      = sizeof (struct component_array) * (ecs->num_component_types + 1);
  ecs->arrays = realloc (ecs->arrays, newsize);
  assert (ecs->arrays != NULL);

  size_t bufsize = size * nmemb;
  size_t mapsize = sizeof (struct offset_pair) * nmemb;
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
  struct component_bitset *bitset = &ecs->bitsets[entity];
  bitset->sets[component / sizeof (*bitset->sets)] |= (1 << component);

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

void
set_bitset(struct component_bitset *bitset, cid_t component)
{
  const size_t set = component / (sizeof (*bitset->sets) * 8);
  const uint64_t shift = component % (sizeof (*bitset->sets) * 8);
  bitset->sets[set] |= (1LU << shift);
}

void
unset_bitset(struct component_bitset *bitset, cid_t component)
{
  const size_t set = component / (sizeof (*bitset->sets) * 8);
  const uint64_t shift = component % (sizeof (*bitset->sets) * 8);
  bitset->sets[set] &= ~(1LU << shift);
}

static int
get_last (struct offset_pair *map, struct component_array *arr)
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
  struct component_bitset *bitset = &ecs->bitsets[entity];
  const size_t x = sizeof (*bitset->sets) * 8;
  bitset->sets[component / x] &= ~(1 << component % x);

  /* TODO: Is this the best way of doing this? */
  struct component_array *array = &ecs->arrays[component];
  for (size_t i = 0; i < array->num_components; i++)
    {
      if (array->map[i].entity == entity)
        {
          struct offset_pair last_mapping;
          assert (get_last (&last_mapping, array) == 0);
          size_t buffer_offset_to_delete = array->map[i].offset;
          memcpy (array->buf + buffer_offset_to_delete,
                  array->buf + last_mapping.offset, array->stride);
          last_mapping.offset = buffer_offset_to_delete;
          array->map[i] = last_mapping;
          --array->num_components;
          /* Start the search again */
          unmap_component (ecs, entity, component);
        }
    }
}

void *
get_component (struct ecs *ecs, eid_t entity, cid_t component)
{
  struct component_array *array = &ecs->arrays[component];
  for (size_t i = 0; i < array->num_components; i++)
    {
      if (array->map[i].entity == entity)
        {
          return array->buf + array->map[i].offset;
        }
    }
  return NULL;
}
