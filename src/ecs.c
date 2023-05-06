#include "ecs.h"

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

#include "buffer.h"

#define ID_UNUSED (1 << 31)
#define ID_MAX (UINT32_MAX - 1)

static uint32_t entity_buf[MAX_NUM_ENTITIES];
static size_t num_entities;

struct index_table {
  uint32_t entity;
  size_t index;
};

// geometry buffer
static component_t geometry_buf[MAX_NUM_GEOMETRIES];
static size_t num_geometries;
static struct index_table geometry_table[MAX_NUM_GEOMETRIES];

// material buffer
static component_t material_buf[MAX_NUM_MATERIALS];
static size_t num_materials;
static struct index_table material_table[MAX_NUM_MATERIALS];

// position buffer
static component_t position_buf[MAX_NUM_MATERIALS];
static size_t num_positions;
static struct index_table position_table[MAX_NUM_MATERIALS];

struct entry {
  component_t *buffer;
  struct index_table *table;
  size_t *counter;
};

static struct entry lookup_table[NUM_COMPONENT_TYPES] = {
    {geometry_buf, geometry_table, &num_geometries},
    {material_buf, material_table, &num_materials},
    {position_buf, position_table, &num_positions}
    // ...
};

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
uint32_t ecs_identity(uint32_t e) { return entity_buf[e]; }

/**
 * ----------------------------------------------------------------------------
 */
int ecs_add_component(uint32_t e, uint32_t type) {
  if ((entity_buf[e] & ID_UNUSED) != 0) {
    return -1;
  }

  uint32_t mask = (1 << type);
  struct entry entry = lookup_table[type];
  if (entity_buf[e] & mask) {
    return 0;
  }

  entity_buf[e] |= mask;
  struct index_table *it = entry.table;
  size_t *counter = entry.counter;
  it[*counter].entity = e;
  it[*counter].index = *counter;
  (*counter)++;

  return 0;
}

/**
 * ----------------------------------------------------------------------------
 */
static int index_by_entity(uint32_t e, struct index_table *table, size_t *index,
                           size_t n) {
  for (size_t i = 0; i < n; i++) {
    if (table[i].entity == e) {
      *index = i;
      return 0;
    }
  }
  return -1;
}

/**
 * ----------------------------------------------------------------------------
 */
void ecs_remove_component(uint32_t e, uint32_t type) {
  if ((entity_buf[e] & ID_UNUSED) != 0) {
    return;
  }

  struct entry entry = lookup_table[type];
  size_t *counter = entry.counter;
  struct index_table *table = entry.table;

  size_t table_index_delete;
  if (index_by_entity(e, table, &table_index_delete, *counter) == -1) {
    return;
  }

  if (table_index_delete == *counter) {
      (*counter)--;
  }

  entity_buf[e] &= ~(1 << type);

  size_t buffer_index_delete = table[table_index_delete].index;
  size_t last = (*counter) - 1;
  entry.buffer[buffer_index_delete] = entry.buffer[last];

  table[last].index = buffer_index_delete;
  table[table_index_delete] = table[last];
  (*counter)--;
}

/**
 * ----------------------------------------------------------------------------
 */
component_t *ecs_component(uint32_t e, uint32_t type) {
  struct entry entry = lookup_table[type];
  size_t *counter = entry.counter;
  struct index_table *table = entry.table;
  size_t index;
  if (index_by_entity(e, table, &index, *counter) == -1) {
      return NULL;
  }

  size_t buffer_index = table[index].index;
  return &entry.buffer[buffer_index];
}

/**
 * ----------------------------------------------------------------------------
 */
int ecs_set_component(uint32_t e, uint32_t type, const void *val) {
  (void)e;
  (void)type;
  (void)val;
  return 0;
}

/**
 * ----------------------------------------------------------------------------
 */

size_t ecs_component_count(uint32_t type) {
    return *lookup_table[type].counter;
}

/**
 * ----------------------------------------------------------------------------
 */
handle_t new_handle(void) {
  static handle_t counter = 1;
  return counter++;
}
