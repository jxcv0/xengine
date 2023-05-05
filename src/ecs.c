#include "ecs.h"

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

#include "buffer.h"

#define ID_UNUSED (1 << 31)
#define ID_MAX (UINT32_MAX - 1)

#define GEOMETRY_MASK (1 << 0)
#define MATERIAL_MASK (1 << 1)
#define POSITION_MASK (1 << 2)

union component {
  struct geometry geometry;
  struct pbr_material material;
  vec3 position;
  // ... 
};

// This table is used to look up a mask based on an index
static const uint32_t mask_table[] = {GEOMETRY_MASK, MATERIAL_MASK, POSITION_MASK};

static uint32_t entity_buf[MAX_NUM_ENTITIES];
static size_t num_entities;

struct index_table {
  uint32_t entity;
  size_t index;
};

static struct geometry geometry_buf[MAX_NUM_GEOMETRIES];
static size_t num_geometries;
static struct index_table geometry_table[MAX_NUM_GEOMETRIES];

/*
static struct pbr_material material_buf[MAX_NUM_MATERIALS];
static size_t num_materials;

static vec3 position_buf[MAX_NUM_POSITIONS];
static size_t num_positions;
*/

/**
 * ----------------------------------------------------------------------------
 */
void ecs_init(void) {
  for (size_t i = 0; i < MAX_NUM_ENTITIES; i++) {
    entity_buf[i] = ID_UNUSED;
  }
}

/**
 * ----------------------------------------------------------------------------
 */
int ecs_create_entity(uint32_t *e) {
  if (e == NULL) {
    errno = EINVAL;
    return -1;
  }

  if (num_entities == MAX_NUM_ENTITIES) {
    // TODO errno?
    return -1;
  }

  for (size_t i = 0; i < MAX_NUM_ENTITIES; i++) {
    if (entity_buf[i] & ID_UNUSED) {
      *e = i;
      entity_buf[i] = 0;
      ++num_entities;
      return 0;
    }
  }
  return -1;
}

/**
 * ----------------------------------------------------------------------------
 */
void ecs_delete_entity(uint32_t e) {
  // TODO delete this entities entries in component buffers
  entity_buf[e] = ID_UNUSED;
  --num_entities;
}

/**
 * ----------------------------------------------------------------------------
 */
uint32_t ecs_archetype(uint32_t e) { return entity_buf[e]; }

/**
 * ----------------------------------------------------------------------------
 */
int ecs_add_component(uint32_t e, enum component_type type) {
  if ((entity_buf[e] & ID_UNUSED) == 0) {
    return -1;
  }

  entity_buf[e] |= mask_table[type];
  // insert in buffer
  geometry_table[num_geometries].entity = e;
  geometry_table[num_geometries].index = num_geometries++;

  return 0;
}

/**
 * ----------------------------------------------------------------------------
 */
handle_t new_handle(void) {
  static handle_t counter = 1;
  return counter++;
}
