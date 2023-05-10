#include "mem.h"

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

static cmpnt_t entity_buf[MAX_NUM_ENTITIES];
static size_t num_entities;

struct index_table {
  uint32_t entity;
  size_t index;
};

// geometry buffer
static union component geometry_buf[MAX_NUM_GEOMETRIES];
static struct index_table geometry_table[MAX_NUM_GEOMETRIES];

// material buffer
static union component material_buf[MAX_NUM_MATERIALS];
static struct index_table material_table[MAX_NUM_MATERIALS];

// position buffer
static union component position_buf[MAX_NUM_MATERIALS];
static struct index_table position_table[MAX_NUM_MATERIALS];

// queued asset buffer
static union component queued_asset_buf[MAX_NUM_MATERIALS];
static struct index_table queued_asset_table[MAX_NUM_MATERIALS];

struct entry {
  union component *buffer;
  struct index_table *table;
  size_t count;
};

static struct entry lookup_table[NUM_COMPONENT_TYPES] = {
    {geometry_buf, geometry_table, 0},
    {material_buf, material_table, 0},
    {position_buf, position_table, 0},
    {queued_asset_buf, queued_asset_table, 0}
    // ...
};

/**
 * ----------------------------------------------------------------------------
 */
static int table_index(uint32_t e, struct index_table *table, size_t *index,
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
static int buffer_index(uint32_t e, struct index_table *table, size_t *index,
                        size_t n) {
  for (size_t i = 0; i < n; i++) {
    if (table[i].entity == e) {
      *index = table[i].index;
      return 0;
    }
  }
  return -1;
}

/**
 * ----------------------------------------------------------------------------
 */
void mem_init(void) {
  for (size_t i = 0; i < MAX_NUM_ENTITIES; i++) {
    entity_buf[i] = ENTITY_UNUSED;
  }
}

/**
 * ----------------------------------------------------------------------------
 */
int mem_create_entity(uint32_t *e) {
  if (e == NULL || num_entities == MAX_NUM_ENTITIES) {
    return -1;
  }

  for (size_t i = 0; i < MAX_NUM_ENTITIES; i++) {
    if (entity_buf[i] & ENTITY_UNUSED) {
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
void mem_delete_entity(uint32_t e) {
  cmpnt_t mask = entity_buf[e];
  for (size_t i = 0; i < NUM_COMPONENT_TYPES; i++) {
    if (mask & (1 << i)) {
      mem_remove_component(e, i);
    }
  }
  entity_buf[e] = ENTITY_UNUSED;
  --num_entities;
}

/**
 * ----------------------------------------------------------------------------
 */
uint32_t mem_identity(uint32_t e) { return entity_buf[e]; }

/**
 * ----------------------------------------------------------------------------
 */
int mem_add_component(uint32_t e, cmpnt_t type) {
  if ((entity_buf[e] & ENTITY_UNUSED) != 0) {
    return -1;
  }

  cmpnt_t mask = (1 << type);
  struct entry *entry = &lookup_table[type];
  if (entity_buf[e] & mask) {
    return 0;
  }

  entity_buf[e] |= mask;
  struct index_table *it = entry->table;
  size_t count = entry->count;
  it[count].entity = e;
  it[count].index = count;
  ++entry->count;

  return 0;
}

/**
 * ----------------------------------------------------------------------------
 */
void mem_remove_component(uint32_t e, cmpnt_t type) {
  if ((entity_buf[e] & ENTITY_UNUSED) != 0) {
    return;
  }

  struct entry *entry = &lookup_table[type];
  struct index_table *table = entry->table;

  size_t table_index_delete;
  if (table_index(e, table, &table_index_delete, entry->count) == -1) {
    return;
  }

  if (table_index_delete != entry->count) {
    entity_buf[e] &= ~(1 << type);

    size_t buffer_index_delete = table[table_index_delete].index;
    size_t last = (entry->count) - 1;
    entry->buffer[buffer_index_delete] = entry->buffer[last];

    table[last].index = buffer_index_delete;
    table[table_index_delete] = table[last];
  }
  --entry->count;
}

/**
 * ----------------------------------------------------------------------------
 */
union component *mem_component(uint32_t e, cmpnt_t type) {
  struct entry entry = lookup_table[type];
  struct index_table *table = entry.table;
  size_t index;
  if (table_index(e, table, &index, entry.count) == -1) {
    return NULL;
  }

  size_t buffer_index = table[index].index;
  return &entry.buffer[buffer_index];
}

/**
 * ----------------------------------------------------------------------------
 */
size_t mem_component_count(cmpnt_t type) { return lookup_table[type].count; }

/**
 * ----------------------------------------------------------------------------
 */
size_t mem_count(cmpnt_t mask) {
  size_t count = 0;
  for (size_t j = 0; j < num_entities; j++) {
    if ((entity_buf[j] & mask) == mask) {
      ++count;
    }
  }
  return count;
}

/**
 * ----------------------------------------------------------------------------
 */
void mem_entities(cmpnt_t mask, uint32_t *arr) {
  size_t idx = 0;
  for (size_t i = 0; i < num_entities; i++) {
    if ((entity_buf[i] & mask) == mask) {
      arr[idx++] = i;
    }
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void mem_array(size_t nent, uint32_t *entities, cmpnt_t type,
               union component *array) {
  struct entry entry = lookup_table[type];
  union component *buffer = entry.buffer;
  struct index_table *table = entry.table;
  for (size_t i = 0; i < nent; i++) {
    size_t index;
    if (buffer_index(entities[i], table, &index, entry.count) == -1) {
      return;
    }
    array[i] = buffer[index];
  }
}

void mem_write(size_t nent, uint32_t *entities, cmpnt_t type,
               union component *array) {
  struct entry entry = lookup_table[type];
  union component *buffer = entry.buffer;
  struct index_table *table = entry.table;
  for (size_t i = 0; i < nent; i++) {
    size_t index;
    if (buffer_index(entities[i], table, &index, entry.count) == -1) {
      return;
    }
    buffer[index] = array[i];
  }
}
