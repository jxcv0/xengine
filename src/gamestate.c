#include "gamestate.h"

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>

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

// rotation buffer
static union component rotation_buf[MAX_NUM_POSITIONS];
static struct index_table rotation_table[MAX_NUM_POSITIONS];

// model matrix buffer
static union component model_matrix_buf[MAX_NUM_MODEL_MATRICES];
static struct index_table model_matrix_table[MAX_NUM_MODEL_MATRICES];

// geometry request buffer
static union component geomreq_buf[MAX_NUM_LOAD_REQUESTS];
static struct index_table geomreq_table[MAX_NUM_LOAD_REQUESTS];

// material request buffer
static union component matreq_buf[MAX_NUM_LOAD_REQUESTS];
static struct index_table matreq_table[MAX_NUM_LOAD_REQUESTS];

struct row {
  union component *buffer;
  struct index_table *table;
  size_t count;
  pthread_mutex_t lock;
};

// TODO should each of these have a mutex?
static struct row lookup_table[NUM_COMPONENT_TYPES] = {
    {mesh_buf, geometry_table, 0, PTHREAD_MUTEX_INITIALIZER},
    {material_buf, material_table, 0, PTHREAD_MUTEX_INITIALIZER},
    {position_buf, position_table, 0, PTHREAD_MUTEX_INITIALIZER},
    {rotation_buf, rotation_table, 0, PTHREAD_MUTEX_INITIALIZER},
    {model_matrix_buf, model_matrix_table, 0, PTHREAD_MUTEX_INITIALIZER},
    {geomreq_buf, geomreq_table, 0, PTHREAD_MUTEX_INITIALIZER},
    {matreq_buf, matreq_table, 0, PTHREAD_MUTEX_INITIALIZER}};

/**
 * ----------------------------------------------------------------------------
 */
static struct row *aquire_row(uint64_t type) {
  struct row *row = &lookup_table[type];
  pthread_mutex_lock(&row->lock);
  return row;
}

/**
 * ----------------------------------------------------------------------------
 */
static void release_row(struct row *row) { pthread_mutex_unlock(&row->lock); }

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
uint64_t get_identity(uint32_t e) { return entity_buf[e]; }

/**
 * ----------------------------------------------------------------------------
 */
int add_component(uint32_t e, uint64_t type) {
  if ((entity_buf[e] & ENTITY_UNUSED) != 0) {
    return -1;
  }

  uint64_t mask = (1LU << type);
  if ((entity_buf[e] & mask) == 0) {
    struct row *row = aquire_row(type);
    entity_buf[e] |= mask;

    struct index_table *it = row->table;
    size_t count = row->count;
    it[count].entity = e;
    it[count].index = count;
    ++row->count;
    release_row(row);
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

  struct row *row = aquire_row(type);
  struct index_table *table = row->table;

  size_t table_index_delete;
  if (table_index(e, table, &table_index_delete, row->count) == -1) {
    release_row(row);
    return;
  }

  if (table_index_delete != row->count) {
    entity_buf[e] &= ~(1LU << type);

    size_t buffer_index_delete = table[table_index_delete].index;
    size_t last = (row->count) - 1;
    row->buffer[buffer_index_delete] = row->buffer[last];

    table[last].index = buffer_index_delete;
    table[table_index_delete] = table[last];
  }
  --row->count;
  release_row(row);
}

int set_component(uint32_t e, uint64_t type, union component cmpnt) {
  struct row *row = aquire_row(type);
  struct index_table *table = row->table;
  size_t index;
  if (buffer_index(e, table, &index, row->count) == -1) {
    release_row(row);
    return -1;
  }
  row->buffer[index] = cmpnt;
  release_row(row);
  return 0;
}

/**
 * ----------------------------------------------------------------------------
 */
union component get_component(uint32_t e, uint64_t type) {
  struct row *row = aquire_row(type);
  struct index_table *table = row->table;
  size_t index;
  assert(buffer_index(e, table, &index, row->count) != -1);
  cmpnt_t c = row->buffer[index];
  release_row(row);
  return c;
}

/**
 * ----------------------------------------------------------------------------
 */
size_t get_component_count(uint64_t type) {
  struct row *row = aquire_row(type);
  uint64_t count = lookup_table[type].count;
  release_row(row);
  return count;
}

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
           union component *set) {
  struct row *row = aquire_row(type);
  union component *buffer = row->buffer;
  struct index_table *table = row->table;
  for (size_t i = 0; i < nent; i++) {
    size_t index;
    assert(buffer_index(entities[i], table, &index, row->count) != -1);
    set[i] = buffer[index];
  }
  release_row(row);
}

/**
 * ----------------------------------------------------------------------------
 */
void update(size_t nent, uint32_t *entities, uint64_t type,
            union component *set) {
  struct row *row = aquire_row(type);
  union component *buffer = row->buffer;
  struct index_table *table = row->table;
  for (size_t i = 0; i < nent; i++) {
    size_t index;
    if (buffer_index(entities[i], table, &index, row->count) == -1) {
      release_row(row);
      return;
    }
    buffer[index] = set[i];
  }
  release_row(row);
}
