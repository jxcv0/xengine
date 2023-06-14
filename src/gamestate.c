#include "gamestate.h"

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>

// bitmasks
#define MESH_BIT (1LU << MESH)
#define MATERIAL_BIT (1LU << MATERIAL)
#define MODEL_MATRIX_BIT (1LU << MODEL_MATRIX)
#define POSITION_BIT (1LU << POSITION)
#define MESH_LOAD_REQUEST_BIT (1LU << MESH_LOAD_REQUEST);
#define MAT_LOAD_REQUEST_BIT (1LU << MAT_LOAD_REQUEST);
// #define ANIM_LOAD_REQUEST_BIT (1LU << ANIM_LOAD_REQUEST);
// #define AUDIO_LOAD_REQUEST_BIT (1LU << AUDIO_LOAD_REQUEST);

static uint64_t entity_buf[MAX_NUM_ENTITIES];
static size_t num_entities;

struct index_table {
  uint32_t entity;
  size_t index;
};

// geometry buffer
static union component mesh_buf[MAX_NUM_GEOMETRIES];
static struct index_table geometry_table[MAX_NUM_GEOMETRIES];

// material buffer
static union component material_buf[MAX_NUM_MATERIALS];
static struct index_table material_table[MAX_NUM_MATERIALS];

// position buffer
static union component position_buf[MAX_NUM_POSITIONS];
static struct index_table position_table[MAX_NUM_POSITIONS];

// model matrix buffer
static union component model_matrix_buf[MAX_NUM_MODEL_MATRICES];
static struct index_table model_matrix_table[MAX_NUM_MODEL_MATRICES];

// geometry request buffer
static union component geomreq_buf[MAX_NUM_LOAD_REQUESTS];
static struct index_table geomreq_table[MAX_NUM_LOAD_REQUESTS];

// material request buffer
static union component matreq_buf[MAX_NUM_LOAD_REQUESTS];
static struct index_table matreq_table[MAX_NUM_LOAD_REQUESTS];

struct entry {
  union component *buffer;
  struct index_table *table;
  size_t count;
};

// TODO should each of these have a mutex?
static struct entry lookup_table[NUM_COMPONENT_TYPES] = {
    {mesh_buf, geometry_table, 0},
    {material_buf, material_table, 0},
    {position_buf, position_table, 0},
    {model_matrix_buf, model_matrix_table, 0},
    {geomreq_buf, geomreq_table, 0},
    {matreq_buf, matreq_table, 0}
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
void init_mem_subsys(void) {
  for (size_t i = 0; i < MAX_NUM_ENTITIES; i++) {
    entity_buf[i] = ENTITY_UNUSED;
  }
}

/**
 * ----------------------------------------------------------------------------
 */
uint64_t create_mask(size_t n, uint64_t *components) {
  uint64_t mask = 0;
  for (size_t i = 0; i < n; i++) {
    mask |= (1LU << components[i]);
  }
  return mask;
}

/**
 * ----------------------------------------------------------------------------
 */
int create_entity(uint32_t *e) {
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
void delete_entity(uint32_t e) {
  uint64_t mask = entity_buf[e];
  for (size_t i = 0; i < NUM_COMPONENT_TYPES; i++) {
    if (mask & (1LU << i)) {
      remove_component(e, i);
    }
  }
  entity_buf[e] = ENTITY_UNUSED;
  --num_entities;
}

/**
 * ----------------------------------------------------------------------------
 */
uint64_t get_identity(uint32_t e) {
  return entity_buf[e];
}

/**
 * ----------------------------------------------------------------------------
 */
int add_component(uint32_t e, uint64_t type) {
  if ((entity_buf[e] & ENTITY_UNUSED) != 0) {
    return -1;
  }

  uint64_t mask = (1LU << type);
  if ((entity_buf[e] & mask) == 0) {
    struct entry *entry = &lookup_table[type];
    entity_buf[e] |= mask;

    struct index_table *it = entry->table;
    size_t count = entry->count;
    it[count].entity = e;
    it[count].index = count;
    ++entry->count;
  }

  return 0;
}

/**
 * ----------------------------------------------------------------------------
 */
void remove_component(uint32_t e, uint64_t type) {
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
    entity_buf[e] &= ~(1LU << type);

    size_t buffer_index_delete = table[table_index_delete].index;
    size_t last = (entry->count) - 1;
    entry->buffer[buffer_index_delete] = entry->buffer[last];

    table[last].index = buffer_index_delete;
    table[table_index_delete] = table[last];
  }
  --entry->count;
}

int set_component(uint32_t e, uint64_t type, union component cmpnt) {
  struct entry entry = lookup_table[type];
  struct index_table *table = entry.table;
  size_t index;
  if (buffer_index(e, table, &index, entry.count) == -1) {
    return -1;
  }
  entry.buffer[index] = cmpnt;
  return 0;
}

/**
 * ----------------------------------------------------------------------------
 */
union component *get_component(uint32_t e, uint64_t type) {
  struct entry entry = lookup_table[type];
  struct index_table *table = entry.table;
  size_t index;
  if (buffer_index(e, table, &index, entry.count) == -1) {
    return NULL;
  }
  return entry.buffer + index;
}

/**
 * ----------------------------------------------------------------------------
 */
size_t get_component_count(uint64_t type) { return lookup_table[type].count; }

/**
 * ----------------------------------------------------------------------------
 */
size_t get_num_entities(uint64_t mask) {
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
void get_entities(uint64_t mask, uint32_t *arr) {
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
void query(size_t nent, uint32_t *entities, uint64_t type,
           union component *array) {
  struct entry entry = lookup_table[type];
  union component *buffer = entry.buffer;
  struct index_table *table = entry.table;
  for (size_t i = 0; i < nent; i++) {
    size_t index;
    assert(buffer_index(entities[i], table, &index, entry.count) != -1);
    array[i] = buffer[index];
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void update(size_t nent, uint32_t *entities, uint64_t type,
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
