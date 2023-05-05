#include "ecs.h"

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

#include "buffer.h"

#define ID_UNUSED (1 << 31)
#define ID_MAX (UINT32_MAX - 1)

#define GEOMETRY_INDEX 0
#define MATERIAL_INDEX 1
#define POSITION_INDEX 2

static struct {
  uint32_t *data;
  size_t count;
  size_t max;
} entity_buf;

struct index_map {
  uint32_t entity;
  size_t index;
};

static struct geometry geometry_buf[MAX_NUM_GEOMETRIES];
static size_t num_geometries;
static struct index_map geometry_map[MAX_NUM_GEOMETRIES];

static struct pbr_material material_buf[MAX_NUM_MATERIALS];
static size_t num_materials;

static vec3 position_buf[MAX_NUM_POSITIONS];
static size_t num_positions;

/**
 * ----------------------------------------------------------------------------
 */
int ecs_init(size_t nent) {
  if (nent >= ID_MAX) {
    return -1;
  }

  entity_buf.data = calloc(nent, sizeof(uint32_t));
  if (entity_buf.data == NULL) {
    return -1;
  }
  for (size_t i = 0; i < nent; i++) {
    entity_buf.data[i] = ID_UNUSED;
  }
  entity_buf.count = 0;
  entity_buf.max = nent;

  return 0;
}

/**
 * ----------------------------------------------------------------------------
 */
int ecs_create_entity(uint32_t *e) {
  if (entity_buf.data == NULL || e == NULL) {
    errno = EINVAL;
    return -1;
  }

  if (entity_buf.count == entity_buf.max) {
    // TODO errno?
    return -1;
  }

  for (size_t i = 0; i < entity_buf.max; i++) {
    if (entity_buf.data[i] & ID_UNUSED) {
      *e = i;
      entity_buf.data[i] = 0;
      entity_buf.count++;
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
  entity_buf.data[e] = ID_UNUSED;
  entity_buf.count--;
}

/**
 * ----------------------------------------------------------------------------
 */
uint32_t ecs_archetype(uint32_t e) { return entity_buf.data[e]; }

/**
 * ----------------------------------------------------------------------------
 */
int ecs_add_component(uint32_t e, uint32_t type) {
  if ((entity_buf.data[e] & ID_UNUSED) == 0) {
    return -1;
  }

  switch (type) {
    case GEOMETRY:
      if (num_geometries != MAX_NUM_GEOMETRIES) {
        geometry_buf[num_geometries++];
      }
      break;

    default:
      return -1;
  }

  entity_buf.data[e] |= type;
  return 0;
}

/**
 * ----------------------------------------------------------------------------
 */
handle_t new_handle(void) {
  static handle_t counter = 1;
  return counter++;
}
