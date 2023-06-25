#include "gamestate.h"

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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

struct component_table {
  union component *buffer;
  struct index_table *table;
  size_t count;
};

// TODO should each of these have a mutex?
static struct component_table lookup_table[NUM_COMPONENT_TYPES] = {
    {mesh_buf, geometry_table, 0},
    {material_buf, material_table, 0},
    {position_buf, position_table, 0},
    {rotation_buf, rotation_table, 0},
    {model_matrix_buf, model_matrix_table, 0},
    {geomreq_buf, geomreq_table, 0},
    {matreq_buf, matreq_table, 0}};

/**
 * ----------------------------------------------------------------------------
 */
gamestate_t create_gamestate(int nobj) {
  gamestate_t g = {
      .nobj = 0, .max_nobj = nobj, .otable = malloc(sizeof(uint64_t) * nobj)};
  memset(g.otable, 0, sizeof(uint64_t) * nobj);
  return g;
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
int create_game_object(gamestate_t *gs) {
  if (gs->nobj < gs->max_nobj) {
    gs->otable[gs->nobj] = 0;
    return gs->nobj++;
  }
  return -1;
}

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
  uint64_t i = entity_buf[e];
  return i;
}

/**
 * ----------------------------------------------------------------------------
 */
int add_component(uint32_t e, uint64_t type) {
  uint64_t identity = get_identity(e);
  if ((identity & ENTITY_UNUSED) != 0) {
    return -1;
  }

  uint64_t mask = (1LU << type);
  if ((identity & mask) == 0) {
    struct component_table *component_table = &lookup_table[type];
    entity_buf[e] |= mask;

    struct index_table *it = component_table->table;
    size_t count = component_table->count;
    it[count].entity = e;
    it[count].index = count;
    ++component_table->count;
  }
  return 0;
}

/**
 * ----------------------------------------------------------------------------
 */
void remove_component(uint32_t e, uint64_t type) {
  uint64_t identity = get_identity(e);
  if ((identity & ENTITY_UNUSED) != 0) {
    return;
  }

  struct component_table *component_table = &lookup_table[type];
  struct index_table *table = component_table->table;

  size_t table_index_delete;
  if (table_index(e, table, &table_index_delete, component_table->count) ==
      -1) {
    return;
  }

  // change indices if we need too
  if (table_index_delete != component_table->count) {
    size_t buffer_index_delete = table[table_index_delete].index;
    size_t last = (component_table->count) - 1;
    component_table->buffer[buffer_index_delete] =
        component_table->buffer[last];

    table[last].index = buffer_index_delete;
    table[table_index_delete] = table[last];
  }

  entity_buf[e] &= ~(1LU << type);

  --component_table->count;
}

/**
 * ----------------------------------------------------------------------------
 */
int set_component(uint32_t e, uint64_t type, union component cmpnt) {
  struct component_table *component_table = &lookup_table[type];
  struct index_table *table = component_table->table;
  size_t index;
  if (buffer_index(e, table, &index, component_table->count) == -1) {
    return -1;
  }
  component_table->buffer[index] = cmpnt;
  return 0;
}

/**
 * ----------------------------------------------------------------------------
 */
union component get_component(uint32_t e, uint64_t type) {
  struct component_table *component_table = &lookup_table[type];
  struct index_table *table = component_table->table;
  size_t index;
  assert(buffer_index(e, table, &index, component_table->count) != -1);
  cmpnt_t c = component_table->buffer[index];
  return c;
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
           union component *set) {
  struct component_table *component_table = &lookup_table[type];
  union component *buffer = component_table->buffer;
  struct index_table *table = component_table->table;
  for (size_t i = 0; i < nent; i++) {
    size_t index;
    assert(buffer_index(entities[i], table, &index, component_table->count) !=
           -1);
    set[i] = buffer[index];
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void update(size_t nent, uint32_t *entities, uint64_t type,
            union component *set) {
  struct component_table *component_table = &lookup_table[type];
  union component *buffer = component_table->buffer;
  struct index_table *table = component_table->table;
  for (size_t i = 0; i < nent; i++) {
    size_t index;
    if (buffer_index(entities[i], table, &index, component_table->count) ==
        -1) {
      return;
    }
    buffer[index] = set[i];
  }
}
